#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <algorithm>
#include <iostream>

struct Song {
    std::string name;
    float volume;
    std::string path;
    std::unique_ptr<sf::Music> music;
    bool isPlaying;
    bool isPaused;
    
    Song() : volume(100), isPlaying(false), isPaused(false) {}
    
    Song(const std::string& n, const std::string& p, float v = 100) 
        : name(n), path(p), volume(v), isPlaying(false), isPaused(false) {
        music = std::make_unique<sf::Music>();
    }
    
    // Move constructor
    Song(Song&& other) noexcept
        : name(std::move(other.name)),
          volume(other.volume),
          path(std::move(other.path)),
          music(std::move(other.music)),
          isPlaying(other.isPlaying),
          isPaused(other.isPaused) {}
    
    // Move assignment
    Song& operator=(Song&& other) noexcept {
        if (this != &other) {
            name = std::move(other.name);
            volume = other.volume;
            path = std::move(other.path);
            music = std::move(other.music);
            isPlaying = other.isPlaying;
            isPaused = other.isPaused;
        }
        return *this;
    }
    
    Song(const Song&) = delete;
    Song& operator=(const Song&) = delete;
};

class AudioManager {
private:
    std::vector<std::unique_ptr<sf::Sound>> activeSounds;
    std::vector<std::unique_ptr<sf::SoundBuffer>> soundBuffers;
    std::map<std::string, Song> songs;
    
    float masterVolume;
    float musicVolume;
    float sfxVolume;
    
    void cleanFinishedSounds() {
        activeSounds.erase(
            std::remove_if(activeSounds.begin(), activeSounds.end(),
                [](const std::unique_ptr<sf::Sound>& sound) {
                    return sound->getStatus() == sf::Sound::Stopped;
                }),
            activeSounds.end()
        );
    }
    
public:
    AudioManager() : masterVolume(100), musicVolume(100), sfxVolume(100) {}
    
    void setup() {
        std::cout << "Audio Manager initialized" << std::endl;
    }
    
    void playSound(const std::string& filepath, float volume = 100) {
        auto buffer = std::make_unique<sf::SoundBuffer>();
        
        if (!buffer->loadFromFile(filepath)) {
            std::cerr << "Erro ao carregar som: " << filepath << std::endl;
            return;
        }
        
        auto sound = std::make_unique<sf::Sound>();
        sound->setBuffer(*buffer);
        sound->setVolume(volume * (sfxVolume/100.0f) * (masterVolume/100.0f));
        sound->play();
        
        soundBuffers.push_back(std::move(buffer));
        activeSounds.push_back(std::move(sound));
        
        cleanFinishedSounds();
    }
    
    void addSong(const std::string& name, const std::string& filepath, float volume = 100) {
        Song newSong(name, filepath, volume);
        
        if (!newSong.music->openFromFile(filepath)) {
            std::cerr << "Erro ao carregar música: " << filepath << std::endl;
            return;
        }
        
        newSong.music->setVolume(volume * (musicVolume/100.0f) * (masterVolume/100.0f));
        songs[name] = std::move(newSong);
    }
    
    void playSong(const std::string& name, bool loop = true) {
        auto it = songs.find(name);
        if (it == songs.end()) {
            std::cerr << "Música não encontrada: " << name << std::endl;
            return;
        }
        
        Song& song = it->second;
        
        if (song.isPaused) {
            song.music->play();
            song.isPaused = false;
            song.isPlaying = true;
            return;
        }
        
        if (song.isPlaying) return;
        
        song.music->setLoop(loop);
        song.music->play();
        song.isPlaying = true;
        song.isPaused = false;
    }
    
    void pauseSong(const std::string& name) {
        auto it = songs.find(name);
        if (it != songs.end()) {
            Song& song = it->second;
            if (song.isPlaying && !song.isPaused) {
                song.music->pause();
                song.isPaused = true;
            }
        }
    }
    
    void stopSong(const std::string& name) {
        auto it = songs.find(name);
        if (it != songs.end()) {
            Song& song = it->second;
            song.music->stop();
            song.isPlaying = false;
            song.isPaused = false;
        }
    }
    
    void deleteSong(const std::string& name) {
        auto it = songs.find(name);
        if (it != songs.end()) {
            it->second.music->stop();
            songs.erase(it);
        }
    }
    
    void resumeSong(const std::string& name) {
        auto it = songs.find(name);
        if (it != songs.end()) {
            Song& song = it->second;
            if (song.isPaused) {
                song.music->play();
                song.isPaused = false;
            }
        }
    }
    
    void pauseAll() {
        for (auto& [name, song] : songs) {
            if (song.isPlaying && !song.isPaused) {
                song.music->pause();
                song.isPaused = true;
            }
        }
    }
    
    void resumeAll() {
        for (auto& [name, song] : songs) {
            if (song.isPaused) {
                song.music->play();
                song.isPaused = false;
            }
        }
    }
    
    void stopAll() {
        for (auto& [name, song] : songs) {
            song.music->stop();
            song.isPlaying = false;
            song.isPaused = false;
        }
        
        activeSounds.clear();
        soundBuffers.clear();
    }
    
    void setMasterVolume(float volume) {
        masterVolume = std::clamp(volume, 0.0f, 100.0f);
        updateAllVolumes();
    }
    
    void setMusicVolume(float volume) {
        musicVolume = std::clamp(volume, 0.0f, 100.0f);
        updateAllVolumes();
    }
    
    void setSFXVolume(float volume) {
        sfxVolume = std::clamp(volume, 0.0f, 100.0f);
        updateAllVolumes();
    }
    
    void updateAllVolumes() {
        for (auto& [name, song] : songs) {
            song.music->setVolume(song.volume * (musicVolume/100.0f) * (masterVolume/100.0f));
        }
    }
    
    float getMasterVolume() const { return masterVolume; }
    float getMusicVolume() const { return musicVolume; }
    float getSFXVolume() const { return sfxVolume; }
    
    bool isSongPlaying(const std::string& name) const {
        auto it = songs.find(name);
        return it != songs.end() && it->second.isPlaying && !it->second.isPaused;
    }
    
    bool isSongPaused(const std::string& name) const {
        auto it = songs.find(name);
        return it != songs.end() && it->second.isPaused;
    }
    
    void update() {
        cleanFinishedSounds();
        
        for (auto& [name, song] : songs) {
            if (song.isPlaying && !song.isPaused) {
                if (song.music->getStatus() == sf::Music::Stopped) {
                    song.isPlaying = false;
                }
            }
        }
    }
};