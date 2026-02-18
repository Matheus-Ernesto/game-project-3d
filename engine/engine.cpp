#include <unordered_set>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

using namespace std;

class Engine
{
public:
    using clock = std::chrono::steady_clock;

    unordered_set<sf::Keyboard::Key> keysDown;
    unordered_set<sf::Mouse::Button> mouseDown;
    sf::Vector2i mousePosition{0, 0};
    float mouseScrollDelta = 0.f;

    inline static chrono::_V2::steady_clock::time_point last;
    inline static chrono::_V2::steady_clock::time_point now;
    inline static chrono::milliseconds fixedStep;

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
        now = chrono::_V2::steady_clock::now();
        canvas3d.apply(window, width, height, showFPS);
        window.resetGLStates();
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
                sf::String(windowName.c_str()),
                sf::Style::Fullscreen, sf::ContextSettings(24));
        }
        else
        {
            window.create(
                sf::VideoMode(width, height),
                sf::String(windowName.c_str()), sf::Style::Default, sf::ContextSettings(24));
        }

        if (!canvas.font.loadFromFile("fonts/arial.ttf")) // HERE YOU CAN GET THE FONT
        {
            std::cerr << "ERROR\n";
        }
        last = chrono::_V2::steady_clock::now();
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
            if (event.type == sf::Event::Resized){

                reshape(event.size.width, event.size.height);
            }
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
        if (quitForce || !window.isOpen())
            return true;
        return false;
    }
    void exit()
    {
        window.close();
    }

    void setStep(int ms)
    {
        fixedStep = chrono::milliseconds(ms);
    }

    bool isStepped()
    {
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - last);
        return duration >= fixedStep;
    }

    void nextStep()
    {
        last = now;
    }
};