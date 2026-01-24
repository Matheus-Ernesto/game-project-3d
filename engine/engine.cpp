// changes: glut -> glfw -> sfml.
// 20 hours.

#include <SFML/Graphics.hpp>

#include <iostream>
#include <stdio.h>
#include <iostream>
#include <functional>
#include <chrono>
#include <fstream>
#include <cstring>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

struct Configs
{
    int widthWindow = 800;
    int heightWindow = 600;
    bool fullscreen = false;
    bool showFPS = true;

    bool operator==(const Configs &other) const
    {
        return widthWindow == other.widthWindow &&
               heightWindow == other.heightWindow &&
               fullscreen == other.fullscreen &&
               showFPS == other.showFPS;
    }
};

class Loader
{
public:
    int saveConfig(Configs config)
    {
        ofstream file("config.txt");
        if (!file)
            return 1;

        file << config.widthWindow << "\n";
        file << config.heightWindow << "\n";
        file << config.fullscreen << "\n";
        file << config.showFPS << "\n";

        file.close();
        return 0;
    }

    Configs loadConfig()
    {
        Configs config;
        ifstream file("config.txt");
        if (!file)
            return config;

        file >> config.widthWindow;
        file >> config.heightWindow;
        file >> config.fullscreen;
        file >> config.showFPS;

        file.close();
        return config;
    }
};

class Canvas
{
public:
    inline static int totalFrames = 0;
    inline static chrono::high_resolution_clock::time_point last = chrono::high_resolution_clock::now();
    inline static int fps = 0;
    inline static sf::Font font;

    static constexpr int ALIGN_CENTER = 0;

    struct Color
    {
        float r, g, b;
    };

    struct Text
    {
        string text;
        string font;
        int size;
    };

    struct Circle
    {
        float radius;
    };

    class Drawable
    {
    public:
        float x;
        float y;
        Color color;

        Text *text = nullptr;
        Circle *circle = nullptr;

        void draw(sf::RenderWindow &window)
        {
            if (text)
            {

                sf::Text text1;
                text1.setFont(font);
                text1.setString(text->text);
                text1.setCharacterSize(text->size); // tamanho em pixels
                text1.setFillColor(sf::Color(
                    static_cast<sf::Uint8>(color.r * 255),
                    static_cast<sf::Uint8>(color.g * 255),
                    static_cast<sf::Uint8>(color.b * 255)));
                text1.setPosition(x, y);

                window.draw(text1);
            }
            else if (circle)
            {
                sf::CircleShape shape(circle->radius);
                shape.setFillColor(sf::Color(
                    static_cast<sf::Uint8>(color.r * 255),
                    static_cast<sf::Uint8>(color.g * 255),
                    static_cast<sf::Uint8>(color.b * 255)));
                shape.setPosition(x, y);
                window.draw(shape);
            }
        }
    };

    vector<Drawable> draws;

    void apply(sf::RenderWindow &window, int width, int height, bool showFps)
    {
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
            text.setCharacterSize(12); // tamanho em pixels
            text.setFillColor(sf::Color::Red);
            text.setPosition(0.0f, 0.0f);
            window.draw(text);
        }

        // DRAW CHILDS 2D
        for (auto &draw : draws)
        {
            float x_temp = draw.x;
            float y_temp = draw.y;
            draw.x = draw.x * (float)width;
            draw.y = draw.y * (float)height;
            draw.draw(window);
            draw.x = x_temp;
            draw.y = y_temp;
        }
    }

    void text(float xpercetage, float ypercetage, const char *text, int size, float r, float g, float b)
    {
        Drawable t_draw;
        t_draw.text = new Text;
        t_draw.color.r = r;
        t_draw.color.g = g;
        t_draw.color.b = b;
        t_draw.text->size = size;
        t_draw.x = xpercetage;
        t_draw.y = ypercetage;
        t_draw.text->text = text;

        draws.push_back(t_draw);
    }
    void circle(float xpercetage, float ypercetage, float radius, int align, float r, float g, float b)
    {
        Drawable t_draw;
        t_draw.circle = new Circle;
        t_draw.color.r = r;
        t_draw.color.g = g;
        t_draw.color.b = b;
        t_draw.circle->radius = radius;
        t_draw.x = xpercetage;
        t_draw.y = ypercetage;

        draws.push_back(t_draw);
    }
    void clearAll()
    {
        draws.clear();
    }
};

class Engine
{
public:
    inline static int timeGameStep = 16;
    inline static bool fullscreen = false;
    inline static int width = 100;
    inline static int height = 100;
    inline static bool showFPS = false;
    inline static string windowName = "Test";
    inline static bool quitForce = false;
    inline static Canvas canvas;
    inline static sf::RenderWindow window;

    static void reshape(int t_width, int t_height)
    {
        width = t_width;
        height = t_height;
    }

    static void display()
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                quitForce = true;
            }
        }

        window.clear();
        canvas.apply(window, width, height, true);
        window.display();
    }

    void start()
    {
        window.create(
            sf::VideoMode(width, height),
            windowName);
        if (!canvas.font.loadFromFile("fonts/arial.ttf"))
        {
            std::cerr << "Erro ao carregar fonte arial.ttf\n";
        }
    }

    bool quit()
    {
        return quitForce;
    }

    void exit()
    {
    }

    void keyboardListener()
    {
    }
    void mouseMoveListener()
    {
    }
    void mouseKeyListener()
    {
    }
    void mouseScrollListener()
    {
    }
};

Engine engine;
Configs configs;
int gameMenu = 0;
int deltaTime = 0;

int main()
{
    Loader loader;
    Configs t_configs = loader.loadConfig();
    if (configs == t_configs)
    {
        loader.saveConfig(configs); // SAVE BECAUSE DOESNT EXIST THE FILE
    }
    else
    {
        configs = t_configs; // IS DIFFERENT FROM NORMAL
    }

    engine.fullscreen = configs.fullscreen;
    engine.width = configs.widthWindow;
    engine.height = configs.heightWindow;
    engine.showFPS = configs.showFPS;

    // Game configs
    engine.timeGameStep = 10; // 60FPS for game steps
    engine.windowName = "Metal Himiko - V0.0";

    engine.start();

    while (!engine.quit())
    {
        deltaTime++;
        switch (gameMenu)
        {
        case 0:
            // TO DO
            if (deltaTime <= 80)
            {
                int fadeIn = deltaTime / 8;
                engine.canvas.clearAll();
                engine.canvas.text(0.5f, 0.5f, "Metal Himiko", 12, 0.1f * fadeIn, 0.1f * fadeIn, 0.1f * fadeIn);
            }
            else if (deltaTime > 240 && deltaTime < 320)
            {
                float fadeOut = (float)abs(deltaTime - 320) / 80.0f;
                engine.canvas.clearAll();
                engine.canvas.text(0.5f, 0.5f, "Metal Himiko",12, fadeOut, fadeOut, fadeOut);
            }
            else if (deltaTime >= 320)
            {
                gameMenu = 1;
                // engine.canvas.text(0.5f,0.5f,"Metal Himiko",0,0.1f*fadeIn,0.1f*fadeIn,0.1f*fadeIn);
            }
            break;
        case 1:
            // deltaTime = 321 now
            if (deltaTime >= 500 && deltaTime < 600)
            {
                float move = (float)(deltaTime - 500) / 100.0f;
                engine.canvas.clearAll();
                engine.canvas.text(0.1f * move, 0.1f, "Metal Himiko", 12, 1.0f * move, 1.0f * move, 0.7f * move);
                engine.canvas.text(0.1f * move, 0.5f, "Iniciar", 12, 1.0f * move, 1.0f * move, 0.7f * move);
                engine.canvas.text(0.1f * move, 0.6f, "Configurações", 12, 1.0f * move, 1.0f * move, 0.7f * move);
            }
            else if (deltaTime = 600)
            {
                engine.canvas.clearAll();
                engine.canvas.text(0.1f, 0.1f, "Metal Himiko", 12, 1.0f, 1.0f, 0.7f);
                engine.canvas.text(0.1f, 0.5f, "Iniciar", 12, 1.0f, 1.0f, 0.7f);
                engine.canvas.text(0.1f, 0.6f, "Configurações", 12, 1.0f, 1.0f, 0.7f);
            }
            break;
        default:
            break;
        }
        engine.display();
    };
    return 0;
}
