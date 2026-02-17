// changes: glut -> glfw -> sfml. Ufff
// 30 hours.

// YES, IS NOT IN ENGLISH THE GAME, JUST THE CODE.
// SOMEDAY I GET A LOCALIZATION SCRIPT
// BUT IS NOT TODAY.

// SEE PLANS.MD TO READ WHERE I LOSE MY MIND

#include <unordered_set>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

// Idk half of this libs, they just appeared because compilers errors.

// I use linux mint without lightdm btw :)

// HALF OF THIS CODE WAS WRITTEN IN VSCODE WITH A KEYBOARD, HALF WITH MY ASS

// CHATGPT DOESNT MAKE THIS CODE, BECAUSE IS TOO TRASHY

// I DONT KNOW WHY AM DOCUMETING THIS SHIT, NO ONES GOES TO READ THIS...

// ALSO I ALREADY SAID I USE LINUX?

// IF IT DOESNT RUN WELL ON WINDOWS, DONT BLAME ME, I DONT HAVE A WINDOWS.

// I DONT CARE ABOUT MACOS

// REQUISITES TO RUN THIS GAME: MY PC...

// E5300, 4GB OF RAM, NO GPU.
//  TODAY IS GETTING 300 FPS. - 2026-01-24.

// TO COMPILE FOR LINUX: g++ engine.cpp -o engine -lsfml-graphics -lsfml-window -lsfml-system
// TO COMPILE FOR WINDOWS (PROBALY, IDK): x86_64-w64-mingw32-g++ engine.cpp -o engine.exe SOME BULLSHIT DLL...

using namespace std;

// SFML MISC AND SETUP
class Engine
{
public:
    unordered_set<sf::Keyboard::Key> keysDown;
    unordered_set<sf::Mouse::Button> mouseDown;
    sf::Vector2i mousePosition{0, 0};
    float mouseScrollDelta = 0.f;

    inline static int timeGameStep = 16;
    inline static bool fullscreen = false;
    inline static int width = 100;
    inline static int height = 100;
    inline static bool showFPS = false;
    inline static string windowName = "Test";
    inline static bool quitForce = false;
    inline static Canvas canvas;
    inline static Canvas3D canvas3d;
    inline static sf::RenderWindow window;

    static void reshape(int t_width, int t_height)
    {
        width = t_width;
        height = t_height;
    }

    static void display()
    {
        window.clear();

        if (canvas3d.enable3D)
        {
            canvas3d.apply(window, width, height, showFPS);
        }
        canvas.apply(window, width, height, showFPS);

        window.display();
    }

    void start()
    {
        if (fullscreen)
        {
            width = sf::VideoMode::getDesktopMode().width;
            height = sf::VideoMode::getDesktopMode().height;

            window.create(
                sf::VideoMode::getDesktopMode(),
                windowName,
                sf::Style::Fullscreen);
        }
        else
        {
            window.create(
                sf::VideoMode(width, height),
                windowName);
        }

        canvas3d.setup(window);

        if (!canvas.font.loadFromFile("fonts/arial.ttf")) // HERE YOU CAN GET THE FONT
        {
            std::cerr << "ERROR\n";
        }
    }

    void pollEvents()
    {
        keysDown.clear();
        mouseDown.clear();
        mouseScrollDelta = 0.f;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                quitForce = true;

            if (event.type == sf::Event::KeyPressed)
                keysDown.insert(event.key.code);

            if (event.type == sf::Event::MouseButtonPressed)
                mouseDown.insert(event.mouseButton.button);

            if (event.type == sf::Event::MouseMoved)
                mousePosition = {event.mouseMove.x, event.mouseMove.y};

            if (event.type == sf::Event::MouseWheelScrolled)
                mouseScrollDelta += event.mouseWheelScroll.delta;
        }
    }

    bool getKeyDown(sf::Keyboard::Key key)
    {
        return keysDown.count(key) > 0;
    }

    bool getKey(sf::Keyboard::Key key)
    {
        return sf::Keyboard::isKeyPressed(key);
    }

    bool getMouseKey(sf::Mouse::Button button)
    {
        return mouseDown.count(button) > 0;
    }

    sf::Vector2i getMouseMove()
    {
        return mousePosition;
    }

    float getMouseScroll()
    {
        return mouseScrollDelta;
    }

    bool quit()
    {
        return quitForce || !window.isOpen();
    }
    void exit()
    {
        window.close();
    }
};