#include <SFML/Graphics.hpp>
#include <memory>
#include <chrono>
#include <vector>
#include <string>
#include <iostream>
#include <typeinfo>

using namespace std;

// Classes auxiliares para armazenar recursos carregados
class Texture2d
{
public:
    string name;
    sf::Texture texture;
};

class Font
{
public:
    string name;
    sf::Font font;
};

// CANVAS FOR DRAWING ON WINDOW
class Canvas
{
public:
    inline static int totalFrames = 0;
    inline static chrono::high_resolution_clock::time_point last = chrono::high_resolution_clock::now();
    inline static int fps = 0;
    inline static vector<Font> fonts;

    Collections2d::Gui gui;  // Agora usa a nova estrutura

    vector<sf::Text> texts;
    vector<sf::CircleShape> circles;
    vector<sf::Sprite> sprites;
    vector<Texture2d> textures;

    static constexpr int ALIGN_CENTER = 0;

    void draw(sf::RenderWindow &window)
    {
        // Desenha textos com ajuste proporcional à tela
        for (auto &text_element : texts)
        {
            float temp_x = text_element.getPosition().x;
            float temp_y = text_element.getPosition().y;
            text_element.setPosition(
                temp_x * window.getSize().x,
                temp_y * window.getSize().y - (text_element.getCharacterSize() / 2));
            window.draw(text_element);
            text_element.setPosition(temp_x, temp_y);
        }
        // Desenha círculos
        for (auto &circle_element : circles)
        {
            float temp_x = circle_element.getPosition().x;
            float temp_y = circle_element.getPosition().y;
            circle_element.setPosition(
                temp_x * window.getSize().x,
                temp_y * window.getSize().y);
            window.draw(circle_element);
            circle_element.setPosition(temp_x, temp_y);
        }
        // Desenha sprites
        for (auto &sprite_element : sprites)
        {
            float temp_x = sprite_element.getPosition().x;
            float temp_y = sprite_element.getPosition().y;
            sprite_element.setPosition(
                temp_x * window.getSize().x,
                temp_y * window.getSize().y);
            window.draw(sprite_element);
            sprite_element.setPosition(temp_x, temp_y);
        }
    }

    void apply(sf::RenderWindow &window, int width, int height, bool showFps)
    {
        if (showFps)
        {
            totalFrames++;
            auto now = chrono::high_resolution_clock::now();
            auto diff = chrono::duration<double>(now - last).count();

            if (diff >= 1.0)
            {
                fps = totalFrames / diff;
                totalFrames = 0;
                last = now;
            }

            sf::Text text;
            text.setFont(fonts[0].font);
            text.setString("FPS: " + to_string(fps));
            text.setCharacterSize(18);
            text.setFillColor(sf::Color::Red);
            text.setPosition(0.0f, 0.0f);
            window.draw(text);
        }
        draw(window);
    }

    void clearAll()
    {
        texts.clear();
        circles.clear();
        sprites.clear();
        textures.clear();
        fonts.clear();
        gui.deleteAll();
    }

    void update()
    {
        texts.clear();
        circles.clear();
        sprites.clear();

        for (auto &obj : gui.objects)
        {
            if (!obj.mesh) continue;

            // Extrai cor do objeto (assumindo v4f com x=r, y=g, z=b, w=a)
            sf::Color color(
                static_cast<sf::Uint8>(obj.texture.color.x * 255),
                static_cast<sf::Uint8>(obj.texture.color.y * 255),
                static_cast<sf::Uint8>(obj.texture.color.z * 255),
                static_cast<sf::Uint8>(obj.texture.color.w * 255)
            );

            // Posição (usamos x e y do transform, z pode ser ignorada)
            float posX = obj.transform.position.x;
            float posY = obj.transform.position.y;

            // Alinhamento
            int align = obj.transform.align;

            // --- Processa Texto ---
            if (auto *textMesh = dynamic_cast<Collections2d::Text*>(obj.mesh))
            {
                sf::Text textElement;

                // Carrega a fonte se necessário
                int fontIndex = -1;
                for (size_t i = 0; i < fonts.size(); ++i)
                {
                    if (fonts[i].name == textMesh->pathFont)
                    {
                        fontIndex = i;
                        break;
                    }
                }
                if (fontIndex == -1)
                {
                    cout << "loading font " << textMesh->pathFont << ".\n";
                    Font fonte;
                    fonte.name = textMesh->pathFont;
                    if (!fonte.font.loadFromFile(textMesh->pathFont))
                    {
                        cerr << "ERROR: Failed to load font " << textMesh->pathFont << "\n";
                        continue;
                    }
                    fonts.push_back(fonte);
                    fontIndex = fonts.size() - 1;
                }

                textElement.setFont(fonts[fontIndex].font);
                sf::String sfText = sf::String::fromUtf8(
                    textMesh->text.begin(),
                    textMesh->text.end());
                textElement.setString(sfText);
                textElement.setCharacterSize(textMesh->fontSize);
                textElement.setFillColor(color);

                // Aplica rotação (usando ângulo em graus, por exemplo, rotation.z)
                textElement.setRotation(obj.transform.rotation.z);

                // Ajusta origem conforme alinhamento
                sf::FloatRect bounds = textElement.getLocalBounds();
                switch (align)
                {
                case 0: // esquerda
                    textElement.setOrigin(0.f, bounds.top);
                    break;
                case 1: // centro
                    textElement.setOrigin(bounds.left + bounds.width / 2.f, bounds.top);
                    break;
                case 2: // direita
                    textElement.setOrigin(bounds.left + bounds.width, bounds.top);
                    break;
                }
                textElement.setPosition(posX, posY);

                texts.push_back(textElement);
            }
            // --- Processa Círculo ---
            else if (auto *circleMesh = dynamic_cast<Collections2d::Circle*>(obj.mesh))
            {
                sf::CircleShape circleElement(circleMesh->radius);
                circleElement.setFillColor(color);

                // Aplica rotação (roda a forma, se necessário)
                circleElement.setRotation(obj.transform.rotation.z);

                sf::FloatRect bounds = circleElement.getLocalBounds();
                switch (align)
                {
                case 0:
                    circleElement.setOrigin(0.f, bounds.top);
                    break;
                case 1:
                    circleElement.setOrigin(bounds.left + bounds.width / 2.f, bounds.top);
                    break;
                case 2:
                    circleElement.setOrigin(bounds.left + bounds.width, bounds.top);
                    break;
                }
                circleElement.setPosition(posX, posY);

                circles.push_back(circleElement);
            }
            // --- Processa Imagem ---
            else if (auto *imageMesh = dynamic_cast<Collections2d::Image*>(obj.mesh))
            {
                Texture2d *texturePtr = nullptr;

                // Procura a textura pelo path
                for (auto &texture : textures)
                {
                    if (texture.name == imageMesh->path)
                    {
                        texturePtr = &texture;
                        break;
                    }
                }

                if (!texturePtr)
                {
                    cout << "loading texture " << imageMesh->path << ".\n";
                    sf::Texture novaTexture;
                    if (novaTexture.loadFromFile(imageMesh->path))
                    {
                        Texture2d textureCreated;
                        textureCreated.name = imageMesh->path;
                        textureCreated.texture = novaTexture;
                        textures.push_back(textureCreated);
                        texturePtr = &textures.back();
                        cout << "Texture loaded successfully!\n";
                    }
                    else
                    {
                        cerr << "ERROR: Failed to load texture " << imageMesh->path << "\n";
                        continue;
                    }
                }

                if (texturePtr)
                {
                    sf::Sprite sprite(texturePtr->texture);

                    // Define a região da textura (uv)
                    sf::IntRect uvRect(
                        imageMesh->coordinatesStart.x,
                        imageMesh->coordinatesStart.y,
                        imageMesh->coordinatesEnd.x - imageMesh->coordinatesStart.x,
                        imageMesh->coordinatesEnd.y - imageMesh->coordinatesStart.y
                    );
                    sprite.setTextureRect(uvRect);

                    // Escala para atingir as dimensões desejadas (width/height do mesh)
                    sprite.setScale(
                        imageMesh->width / uvRect.width,
                        imageMesh->height / uvRect.height
                    );

                    // Cor combinando a cor do objeto com a transparência individual da imagem
                    sf::Color spriteColor = color;
                    spriteColor.a = static_cast<sf::Uint8>(color.a * imageMesh->transparency / 255.f);
                    sprite.setColor(spriteColor);

                    // Aplica rotação
                    sprite.setRotation(obj.transform.rotation.z);

                    // Origem conforme alinhamento
                    sf::FloatRect bounds = sprite.getLocalBounds();
                    switch (align)
                    {
                    case 0:
                        sprite.setOrigin(0.f, bounds.top);
                        break;
                    case 1:
                        sprite.setOrigin(bounds.left + bounds.width / 2.f, bounds.top);
                        break;
                    case 2:
                        sprite.setOrigin(bounds.left + bounds.width, bounds.top);
                        break;
                    }
                    sprite.setPosition(posX, posY);

                    sprites.push_back(sprite);
                }
            }
            // Outros tipos de mesh podem ser adicionados aqui futuramente
        }
    }

    void loadFont(string path)
    {
        bool loadedFont = false;
        for (auto font : fonts)
        {
            if (font.name == path)
            {
                loadedFont = true;
                break;
            }
        }
        if (!loadedFont)
        {
            cout << "loading " << path << ".\n";
            Font fonte;
            fonte.name = path;
            if (!fonte.font.loadFromFile(path))
            {
                std::cerr << "ERROR\n";
            }
            fonts.push_back(fonte);
        }
    }

    void loadTexture(string path)
    {
        bool loadedTexture = false;
        for (auto &texture : textures)
        {
            if (texture.name == path)
            {
                loadedTexture = true;
                break;
            }
        }

        if (!loadedTexture)
        {
            cout << "loading texture " << path << ".\n";
            sf::Texture novaTexture;
            if (novaTexture.loadFromFile(path))
            {
                Texture2d textureCreated;
                textureCreated.name = path;
                textureCreated.texture = novaTexture;
                textures.push_back(textureCreated);
                cout << "Texture loaded successfully!\n";
            }
            else
            {
                std::cerr << "ERROR: Failed to load texture " << path << "\n";
            }
        }
    }
};