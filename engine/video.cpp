#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <iostream>
#include <filesystem>
#include <algorithm>

// Estrutura para representar um pack de frames
struct FramePack {
    std::string path;           // pasta dos frames (ex: "assets/frames")
    std::string nameframes;      // nome base (ex: "frame")
    std::string format;          // formato (ex: "png")
    int totalFrames;             // número total de frames
    std::vector<int> frameNumbers; // números dos frames (para ordenação correta)
    
    FramePack() : totalFrames(0) {}
    
    FramePack(const std::string& p, const std::string& n, const std::string& f) 
        : path(p), nameframes(n), format(f), totalFrames(0) {}
};

// Estrutura para vídeo em reprodução
struct PlayingVideo {
    std::string name;
    std::vector<sf::Texture> frames;
    int currentFrame;
    float frameRate;
    sf::Clock frameClock;
    bool isPlaying;
    bool isPaused;
    float frameDuration;
    
    PlayingVideo() : currentFrame(0), frameRate(30), isPlaying(false), 
                     isPaused(false), frameDuration(1.0f/30.0f) {}
};

class VideoManager {
private:
    std::map<std::string, FramePack> framePacks;        // Packs carregados
    std::map<std::string, PlayingVideo> activeVideos;    // Vídeos em reprodução
    
    // Função auxiliar para extrair número do nome do arquivo
    int extractNumber(const std::string& filename, const std::string& prefix) {
        std::string number = filename.substr(prefix.length());
        // Remove a extensão
        size_t dotPos = number.find('.');
        if (dotPos != std::string::npos) {
            number = number.substr(0, dotPos);
        }
        return std::stoi(number);
    }
    
    // Ordena frames numericamente (frame1, frame2, frame10...)
    std::vector<std::string> getSortedFrameFiles(const FramePack& pack) {
        namespace fs = std::filesystem;
        std::vector<std::string> files;
        
        try {
            for (const auto& entry : fs::directory_iterator(pack.path)) {
                if (entry.is_regular_file()) {
                    std::string filename = entry.path().filename().string();
                    // Verifica se começa com o nome base e tem a extensão correta
                    if (filename.find(pack.nameframes) == 0 && 
                        filename.length() > pack.nameframes.length() &&
                        filename.substr(filename.length() - pack.format.length()) == pack.format) {
                        files.push_back(filename);
                    }
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Erro ao ler diretório: " << e.what() << std::endl;
        }
        
        // Ordena numericamente
        std::sort(files.begin(), files.end(), 
            [this, &pack](const std::string& a, const std::string& b) {
                int numA = extractNumber(a, pack.nameframes);
                int numB = extractNumber(b, pack.nameframes);
                return numA < numB;
            });
        
        return files;
    }
    
public:
    void setup() {
        std::cout << "Video Manager initialized" << std::endl;
    }
    
    // Adiciona um frame pack à biblioteca
    void addFramePack(const std::string& packName, const FramePack& pack) {
        framePacks[packName] = pack;
        std::cout << "FramePack '" << packName << "' adicionado" << std::endl;
    }
    
    // Cria um frame pack (versão simplificada)
    void createFramePack(const std::string& packName, 
                         const std::string& path,
                         const std::string& nameframes = "frame",
                         const std::string& format = "png") {
        FramePack newPack(path, nameframes, format);
        
        // Conta quantos frames existem
        auto files = getSortedFrameFiles(newPack);
        newPack.totalFrames = files.size();
        
        // Extrai números para referência
        for (const auto& file : files) {
            newPack.frameNumbers.push_back(extractNumber(file, nameframes));
        }
        
        framePacks[packName] = newPack;
        std::cout << "FramePack '" << packName << "' criado com " 
                  << newPack.totalFrames << " frames" << std::endl;
    }
    
    // Pré-carrega um frame pack na memória
    bool preloadFramePack(const std::string& packName) {
        auto packIt = framePacks.find(packName);
        if (packIt == framePacks.end()) {
            std::cerr << "FramePack não encontrado: " << packName << std::endl;
            return false;
        }
        
        const FramePack& pack = packIt->second;
        auto files = getSortedFrameFiles(pack);
        
        // Cria um PlayingVideo temporário só com as texturas
        PlayingVideo video;
        video.frames.resize(files.size());
        
        for (size_t i = 0; i < files.size(); i++) {
            std::string fullPath = pack.path + "/" + files[i];
            if (!video.frames[i].loadFromFile(fullPath)) {
                std::cerr << "Erro ao carregar frame: " << fullPath << std::endl;
                return false;
            }
        }
        
        // Guarda como um vídeo parado
        activeVideos[packName] = std::move(video);
        std::cout << "FramePack '" << packName << "' pré-carregado com " 
                  << files.size() << " frames" << std::endl;
        return true;
    }
    
    // Toca um frame pack como vídeo
    void playFramePack(const std::string& packName, float framerate = 30, 
                       const std::string& videoName = "") {
        std::string finalName = videoName.empty() ? packName : videoName;
        
        auto packIt = framePacks.find(packName);
        if (packIt == framePacks.end()) {
            std::cerr << "FramePack não encontrado: " << packName << std::endl;
            return;
        }
        
        // Se já existe um vídeo ativo com este nome, para ele primeiro
        if (activeVideos.find(finalName) != activeVideos.end()) {
            stopFramePack(finalName);
        }
        
        const FramePack& pack = packIt->second;
        auto files = getSortedFrameFiles(pack);
        
        PlayingVideo newVideo;
        newVideo.name = finalName;
        newVideo.frameRate = framerate;
        newVideo.frameDuration = 1.0f / framerate;
        newVideo.isPlaying = true;
        newVideo.isPaused = false;
        newVideo.currentFrame = 0;
        newVideo.frames.resize(files.size());
        
        // Carrega todos os frames
        for (size_t i = 0; i < files.size(); i++) {
            std::string fullPath = pack.path + "/" + files[i];
            if (!newVideo.frames[i].loadFromFile(fullPath)) {
                std::cerr << "Erro ao carregar frame: " << fullPath << std::endl;
                return;
            }
        }
        
        newVideo.frameClock.restart();
        activeVideos[finalName] = std::move(newVideo);
        std::cout << "Reproduzindo '" << finalName << "' com " 
                  << files.size() << " frames a " << framerate << " fps" << std::endl;
    }
    
    // Pausa um vídeo
    void pauseFramePack(const std::string& name) {
        auto it = activeVideos.find(name);
        if (it != activeVideos.end() && it->second.isPlaying) {
            it->second.isPaused = true;
            it->second.isPlaying = false;
            std::cout << "Vídeo '" << name << "' pausado" << std::endl;
        }
    }
    
    // Resume um vídeo pausado
    void resumeFramePack(const std::string& name) {
        auto it = activeVideos.find(name);
        if (it != activeVideos.end() && it->second.isPaused) {
            it->second.isPaused = false;
            it->second.isPlaying = true;
            it->second.frameClock.restart();
            std::cout << "Vídeo '" << name << "' resumido" << std::endl;
        }
    }
    
    // Para um vídeo
    void stopFramePack(const std::string& name) {
        auto it = activeVideos.find(name);
        if (it != activeVideos.end()) {
            activeVideos.erase(it);
            std::cout << "Vídeo '" << name << "' parado" << std::endl;
        }
    }
    
    // Desenha um frame específico de um pack (sem reprodução)
    sf::Sprite drawFrame(const std::string& packName, int frameNumber) {
        sf::Sprite sprite;
        
        auto packIt = framePacks.find(packName);
        if (packIt == framePacks.end()) {
            std::cerr << "FramePack não encontrado: " << packName << std::endl;
            return sprite;
        }
        
        const FramePack& pack = packIt->second;
        
        // Constrói o nome do arquivo (sem zeros à esquerda)
        std::string filename = pack.path + "/" + 
                               pack.nameframes + 
                               std::to_string(frameNumber) + 
                               "." + pack.format;
        
        sf::Texture texture;
        if (texture.loadFromFile(filename)) {
            sprite.setTexture(texture);
        } else {
            std::cerr << "Erro ao carregar frame: " << filename << std::endl;
        }
        
        return sprite;
    }
    
    // Desenha um frame específico de um vídeo ativo
    sf::Sprite drawFrameFromVideo(const std::string& name, int frameNumber) {
        sf::Sprite sprite;
        
        auto it = activeVideos.find(name);
        if (it != activeVideos.end() && frameNumber < it->second.frames.size()) {
            sprite.setTexture(it->second.frames[frameNumber]);
        }
        
        return sprite;
    }
    
    // Atualiza todos os vídeos (chamar a cada frame)
    void update() {
        for (auto& [name, video] : activeVideos) {
            if (video.isPlaying && !video.isPaused) {
                if (video.frameClock.getElapsedTime().asSeconds() >= video.frameDuration) {
                    video.currentFrame++;
                    video.frameClock.restart();
                    
                    // Loop quando chegar ao final
                    if (video.currentFrame >= video.frames.size()) {
                        video.currentFrame = 0;
                    }
                }
            }
        }
    }
    
    // Desenha um vídeo ativo na tela
    void draw(sf::RenderWindow& window, const std::string& name, 
              float x = 0, float y = 0, float scaleX = 1, float scaleY = 1) {
        auto it = activeVideos.find(name);
        if (it != activeVideos.end() && !it->second.frames.empty()) {
            sf::Sprite sprite(it->second.frames[it->second.currentFrame]);
            sprite.setPosition(x, y);
            sprite.setScale(scaleX, scaleY);
            window.draw(sprite);
        }
    }
    
    // Getters
    bool isPlaying(const std::string& name) const {
        auto it = activeVideos.find(name);
        return it != activeVideos.end() && it->second.isPlaying;
    }
    
    bool isPaused(const std::string& name) const {
        auto it = activeVideos.find(name);
        return it != activeVideos.end() && it->second.isPaused;
    }
    
    int getCurrentFrame(const std::string& name) const {
        auto it = activeVideos.find(name);
        return it != activeVideos.end() ? it->second.currentFrame : -1;
    }
    
    int getTotalFrames(const std::string& packName) const {
        auto it = framePacks.find(packName);
        return it != framePacks.end() ? it->second.totalFrames : 0;
    }
    
    // Libera recursos
    void cleanup() {
        activeVideos.clear();
        framePacks.clear();
    }
};