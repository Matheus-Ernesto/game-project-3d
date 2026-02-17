#include <SFML/Graphics.hpp>
#include <memory>
#include <chrono>

using namespace std;

// CANVAS FOR DRAWING ON WINDOW
class Canvas
{
public:
    inline static int totalFrames = 0;
    inline static chrono::high_resolution_clock::time_point last = chrono::high_resolution_clock::now();
    inline static int fps = 0;
    inline static sf::Font font;
    vector<sf::Text> texts;
    vector<sf::CircleShape> circles;
    vector<sf::Sprite> sprites;
    vector<unique_ptr<sf::Texture>> textures;

    static constexpr int ALIGN_CENTER = 0;

    struct Color
    {
        float r, g, b;
    };

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
        window.resetGLStates();
        // FPS
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

    void text(float xpercetage, float ypercetage, string text, int size, int position, float r, float g, float b)
    {
        sf::Text text1;
        text1.setFont(font);
        sf::String sfText = sf::String::fromUtf8(
            text.begin(),
            text.end());
        text1.setString(sfText);
        text1.setCharacterSize(size);
        text1.setFillColor(sf::Color(
            static_cast<sf::Uint8>(r * 255),
            static_cast<sf::Uint8>(g * 255),
            static_cast<sf::Uint8>(b * 255)));
        sf::FloatRect bounds = text1.getLocalBounds();
        switch (position)
        {
        case 0:
            text1.setOrigin(0.f, bounds.top);
            break;

        case 1:
            text1.setOrigin(bounds.left + bounds.width / 2.f, bounds.top);
            break;

        case 2:
            text1.setOrigin(bounds.left + bounds.width, bounds.top);
            break;
        }
        text1.setPosition(xpercetage, ypercetage);

        texts.push_back(text1);
    }

    void circle(float xpercetage, float ypercetage, float radius, int align, float r, float g, float b)
    {
        sf::CircleShape shape(radius);
        shape.setFillColor(sf::Color(
            static_cast<sf::Uint8>(r * 255),
            static_cast<sf::Uint8>(g * 255),
            static_cast<sf::Uint8>(b * 255)));
        sf::FloatRect bounds = shape.getLocalBounds();
        switch (align)
        {
        case 0:
            shape.setOrigin(0.f, bounds.top);
            break;

        case 1:
            shape.setOrigin(bounds.left + bounds.width / 2.f, bounds.top);
            break;

        case 2:
            shape.setOrigin(bounds.left + bounds.width, bounds.top);
            break;
        }
        shape.setPosition(xpercetage, ypercetage);

        circles.push_back(shape);
    }

    void image(float xpercetage, float ypercetage, string path, int align, int x1, int y1, int x2, int y2, int width, int height, float transparency)
    {
        auto texture = make_unique<sf::Texture>();
        if (!texture->loadFromFile(path))
        {
            std::cerr << "Error" << std::endl;
        }

        sf::Sprite sprite(*texture);
        sf::IntRect uvRect(x1, y1, x2, y2);
        sprite.setTextureRect(uvRect);
        sprite.setScale((float)width / uvRect.width, (float)height / uvRect.height);
        sprite.setColor(sf::Color(255, 255, 255, transparency));
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
        sprite.setPosition(xpercetage, ypercetage);

        textures.push_back(move(texture));
        sprites.push_back(sprite);
    }

    void clearAll()
    {
        texts.clear();
        circles.clear();
        sprites.clear();
        textures.clear();
    }
};
