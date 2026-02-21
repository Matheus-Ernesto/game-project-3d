#include <SFML/Graphics.hpp>
#include <memory>
#include <chrono>

using namespace std;

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

    GUI gui;

    vector<sf::Text> texts;
    vector<sf::CircleShape> circles;
    vector<sf::Sprite> sprites;
    vector<Texture2d> textures;

    static constexpr int ALIGN_CENTER = 0;

    void draw(sf::RenderWindow &window)
    {
        for (auto &text_element : texts)
        {
            float temp_x = text_element.getPosition().x;
            float temp_y = text_element.getPosition().y;
            text_element.setPosition(
                temp_x * window.getSize().x,
                temp_y * window.getSize().y - (text_element.getCharacterSize() / 2));
            window.draw(text_element);
            text_element.setPosition(
                temp_x,
                temp_y);
        }
        for (auto &circle_element : circles)
        {
            float temp_x = circle_element.getPosition().x;
            float temp_y = circle_element.getPosition().y;
            circle_element.setPosition(
                temp_x * window.getSize().x,
                temp_y * window.getSize().y);
            window.draw(circle_element);
            circle_element.setPosition(
                temp_x,
                temp_y);
        }
        for (auto &sprite_element : sprites)
        {
            float temp_x = sprite_element.getPosition().x;
            float temp_y = sprite_element.getPosition().y;
            sprite_element.setPosition(
                temp_x * window.getSize().x,
                temp_y * window.getSize().y);
            window.draw(sprite_element);
            sprite_element.setPosition(
                temp_x,
                temp_y);
        }
    };

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
    }

    void update()
    {
        texts.clear();
        circles.clear();
        sprites.clear();

        for (auto &originalOBJ : gui.texts)
        {
            sf::Text textElement;
            bool loadedFont = false;
            int pos = 0;
            for (auto font : fonts)
            {
                pos++;
                if (font.name == originalOBJ.pathFont)
                {
                    loadedFont = true;
                    pos--;
                    break;
                }
            }
            if (!loadedFont)
            {
                cout << "loading " << originalOBJ.pathFont << ".\n";
                Font fonte;
                fonte.name = originalOBJ.pathFont;
                if (!fonte.font.loadFromFile(originalOBJ.pathFont)) // HERE YOU CAN GET THE FONT
                {
                    std::cerr << "ERROR\n";
                }
                fonts.push_back(fonte);
                pos = fonts.size();
                pos--;
            }

            textElement.setFont(fonts[pos].font);
            sf::String sfText = sf::String::fromUtf8(
                originalOBJ.text.begin(),
                originalOBJ.text.end());
            textElement.setString(sfText);
            textElement.setCharacterSize(originalOBJ.fontSize);
            textElement.setFillColor(sf::Color(
                static_cast<sf::Uint8>(originalOBJ.r * 255),
                static_cast<sf::Uint8>(originalOBJ.g * 255),
                static_cast<sf::Uint8>(originalOBJ.b * 255)));
            sf::FloatRect bounds = textElement.getLocalBounds();
            switch (originalOBJ.align)
            {
            case 0:
                textElement.setOrigin(0.f, bounds.top);
                break;

            case 1:
                textElement.setOrigin(bounds.left + bounds.width / 2.f, bounds.top);
                break;

            case 2:
                textElement.setOrigin(bounds.left + bounds.width, bounds.top);
                break;
            }
            textElement.setPosition(originalOBJ.x, originalOBJ.y);

            texts.push_back(textElement);
        }
        for (auto &originalOBJ : gui.circles)
        {
            sf::CircleShape circleElement(originalOBJ.radius);
            circleElement.setFillColor(sf::Color(
                static_cast<sf::Uint8>(originalOBJ.r * 255),
                static_cast<sf::Uint8>(originalOBJ.g * 255),
                static_cast<sf::Uint8>(originalOBJ.b * 255)));
            sf::FloatRect bounds = circleElement.getLocalBounds();
            switch (originalOBJ.align)
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
            circleElement.setPosition(originalOBJ.x, originalOBJ.y);

            circles.push_back(circleElement);
        }

        for (auto &originalOBJ : gui.images)
        {
            Texture2d *texturePtr = nullptr;

            // Procura a textura
            for (auto &texture : textures)
            {
                if (texture.name == originalOBJ.path)
                {
                    texturePtr = &texture;
                    break;
                }
            }

            // Se não encontrou, carrega
            if (!texturePtr)
            {
                cout << "loading texture " << originalOBJ.path << ".\n";
                sf::Texture novaTexture;

                if (novaTexture.loadFromFile(originalOBJ.path))
                {
                    Texture2d textureCreated;
                    textureCreated.name = originalOBJ.path;
                    textureCreated.texture = novaTexture;

                    textures.push_back(textureCreated);
                    texturePtr = &textures.back(); // Ponteiro para a nova textura
                    cout << "Texture loaded successfully!\n";
                }
                else
                {
                    std::cerr << "ERROR: Failed to load texture " << originalOBJ.path << std::endl;
                    continue;
                }
            }

            // Cria o sprite
            if (texturePtr)
            {
                sf::Sprite sprite(texturePtr->texture);
                sf::IntRect uvRect(originalOBJ.x1, originalOBJ.y1, originalOBJ.x2, originalOBJ.y2);
                sprite.setTextureRect(uvRect);
                sprite.setScale((float)originalOBJ.width / uvRect.width, (float)originalOBJ.height / uvRect.height);
                sprite.setColor(sf::Color(255, 255, 255, originalOBJ.transparency));

                sf::FloatRect bounds = sprite.getLocalBounds();
                switch (originalOBJ.align)
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
                sprite.setPosition(originalOBJ.x, originalOBJ.y);

                sprites.push_back(sprite);
            }
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
            }
        }
        if (!loadedFont)
        {
            cout << "loading " << path << ".\n";
            Font fonte;
            fonte.name = path;
            if (!fonte.font.loadFromFile(path)) // HERE YOU CAN GET THE FONT
            {
                std::cerr << "ERROR\n";
            }
            fonts.push_back(fonte);
        }
    }
    void loadTexture(string path)
    {
        bool loadedTexture = false;
        for (auto &texture : textures) // Use & para evitar cópia
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
