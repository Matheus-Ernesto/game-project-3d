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

// CANVAS FOR DRAWING ON WINDOW
class Canvas
{
public:
    inline static int totalFrames = 0;
    inline static chrono::high_resolution_clock::time_point last = chrono::high_resolution_clock::now();
    inline static int fps = 0;
    inline static sf::Font font;

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
            text.setFont(font);
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
            textElement.setFont(font);
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
            for (auto &textureLoaded : textures)
            {
                if (textureLoaded.name == originalOBJ.path)
                {
                    sf::Sprite sprite(textureLoaded.texture);
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
                else
                {

                    sf::Texture texture;
                    if (!texture.loadFromFile(originalOBJ.path))
                    {
                        std::cerr << "Error" << std::endl;
                    }

                    sf::Sprite sprite(texture);
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

                    Texture2d textureCreated;

                    textureCreated.name = originalOBJ.path;
                    textureCreated.texture = texture;

                    textures.push_back(textureCreated);
                    sprites.push_back(sprite);
                }
            }
        }
    }
};
