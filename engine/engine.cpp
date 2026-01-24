// changes: glut -> glfw -> sfml. Ufff
// 22 hours.

//YES, IS NOT IN ENGLISH THE GAME, JUST THE CODE.
//SOMEDAY I GET A LOCALIZATION SCRIPT
//BUT IS NOT TODAY.

//SEE PLANS.MD TO READ WHERE I LOSE MY MIND

#include <unordered_set>

#include <SFML/Graphics.hpp>

//Idk half of this libs, they just appeared because compilers errors.

//I use linux mint without lightdm btw :)

//HALF OF THIS CODE WAS WRITTEN IN VSCODE WITH A KEYBOARD, HALF WITH MY ASS

//CHATGPT DOESNT MAKE THIS CODE, BECAUSE IS TOO TRASHY

//I DONT KNOW WHY AM DOCUMETING THIS SHIT, NO ONES GOES TO READ THIS...

//ALSO I ALREADY SAID I USE LINUX?



//IF IT DOESNT RUN WELL ON WINDOWS, DONT BLAME ME, I DONT HAVE A WINDOWS.

//I DONT CARE ABOUT MACOS

//REQUISITES TO RUN THIS GAME: MY PC...

//E5300, 4GB OF RAM, NO GPU.
// TODAY IS GETTING 300 FPS. - 2026-01-24.

//TO COMPILE FOR LINUX: g++ engine.cpp -o engine -lsfml-graphics -lsfml-window -lsfml-system
//TO COMPILE FOR WINDOWS (PROBALY, IDK): x86_64-w64-mingw32-g++ engine.cpp -o engine.exe SOME BULLSHIT DLL...
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

//BASIC CONFIGS FOR ENGINE
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

//CONFIG LOADER FOR CONFIG.TXT
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

//CANVAS FOR DRAWING ON WINDOW

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
        int position;
    };

    struct Circle
    {
        float radius;
    };

    class Drawable  //THIS PROBABLY WILL FILL YOUR RAM
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

                sf::String sfText = sf::String::fromUtf8(
                    text->text.begin(),
                    text->text.end());

                text1.setString(sfText);
                text1.setCharacterSize(text->size);
                text1.setFillColor(sf::Color(
                    static_cast<sf::Uint8>(color.r * 255),
                    static_cast<sf::Uint8>(color.g * 255),
                    static_cast<sf::Uint8>(color.b * 255)));
                sf::FloatRect bounds = text1.getLocalBounds();

                switch (text->position)
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
            text.setCharacterSize(18);
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

    void text(float xpercetage, float ypercetage, const char *text, int size, int position, float r, float g, float b)
    {
        Drawable t_draw;
        t_draw.text = new Text;
        t_draw.color.r = r;
        t_draw.color.g = g;
        t_draw.color.b = b;
        t_draw.text->size = size;
        t_draw.text->position = position;
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

//SFML MISC AND SETUP
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
    inline static sf::RenderWindow window;

    static void reshape(int t_width, int t_height)
    {
        width = t_width;
        height = t_height;
    }

    static void display()
    {
        window.clear();
        canvas.apply(window, width, height, true);
        window.display();
    }

    void start()
    {
        window.create(
            sf::VideoMode(width, height),
            windowName);
        if (!canvas.font.loadFromFile("fonts/arial.ttf")) //HERE YOU CAN GET THE FONT
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

//GAME.CPP

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

    //CONFIG TO ENGINE

    engine.fullscreen = configs.fullscreen;
    engine.width = configs.widthWindow;
    engine.height = configs.heightWindow;
    engine.showFPS = configs.showFPS;

    // Game configs
    engine.timeGameStep = 10; // 60FPS for game steps
    engine.windowName = "Metal Himiko - V0.0";

    engine.start();
    
    //DUAL CORE??? NEVER HEARD OF THAT, JUST PUT A TIMER ON ONE CORE AND EXECUTE 
    //ALL CODE WITH JUST ONE READER, ANYWAYS NO ONE LIKES ATOMIC VARIABLES RIGHT?
    //MAYBE LATER I TEST TO PUT THE RENDER ON OTHER CORE, BUT IM AFRAID OF
    //ACESS SYNC

    using clock = std::chrono::steady_clock;

    auto lastLoop = clock::now();
    auto nowLoop = std::chrono::milliseconds(0);
    const auto fixedStep = std::chrono::milliseconds(10);

    //SAVING STATE ON VARIABLES TO NOT RESTART ON LOOP, BUT I PREFER TO NOT USE A LOOP

    struct u_stateGame1
    {
        int menu = 0;
    };

    struct u_stateGame2 //IS NOT EVEN THE SAME!!!11!! UHUU
    {
        int menu = 0;
    };

    u_stateGame1 stateGame1;
    u_stateGame2 stateGame2;

    while (!engine.quit()) // WHILE TRUE ??
    {
        auto now = clock::now();
        auto frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastLoop);
        lastLoop = now;

        nowLoop += frameTime;

        // FIXED UPDATE (10ms) 
        while (nowLoop >= fixedStep) // 0-10 >= 10 //WHILE INSIDE WHILE, GOOD MOVE MA MAN
        {
            engine.pollEvents();
            deltaTime++; //UN1TY COPY???

            switch (gameMenu)
            {
            case 0: //INTRO
                if (deltaTime <= 80)
                {
                    int fadeIn = deltaTime / 8;
                    engine.canvas.clearAll();
                    engine.canvas.text(0.5f, 0.5f, "Metal Himiko", 48, 1,
                                       0.1f * fadeIn, 0.1f * fadeIn, 0.1f * fadeIn);
                }
                else if (deltaTime > 240 && deltaTime < 320)
                {
                    float fadeOut = abs(deltaTime - 320) / 80.0f;
                    engine.canvas.clearAll();
                    engine.canvas.text(0.5f, 0.5f, "Metal Himiko", 48, 1,
                                       fadeOut, fadeOut, fadeOut);
                }
                else if (deltaTime >= 320)
                {
                    gameMenu = 1;
                    deltaTime = 0;
                }
                break;

            case 1: //PRINCIPAL MENU
                if (deltaTime >= 100 && deltaTime < 200)
                {
                    float move = (deltaTime - 100) / 100.0f;
                    engine.canvas.clearAll();
                    engine.canvas.text(0.1f * move, 0.1f, "Metal Himiko", 36, 0,
                                       move, move, move);
                    engine.canvas.text(0.1f * move, 0.5f, "Iniciar", 24, 0,
                                       move, move, move);
                    engine.canvas.text(0.1f * move, 0.6f, "Configurações", 24, 0,
                                       move, move, move);
                    engine.canvas.text(0.1f * move, 0.7f, "Sair", 24, 0, move, move, move);
                }
                else if (deltaTime >= 200)
                {

                    engine.canvas.clearAll();
                    engine.canvas.text(0.1f, 0.1f, "Metal Himiko", 36, 0, 1.f, 1.f, 1.f);

                    if (engine.getKeyDown(sf::Keyboard::W) || engine.getKeyDown(sf::Keyboard::Up))
                    {
                        stateGame1.menu--;
                    }
                    if (engine.getKeyDown(sf::Keyboard::S) || engine.getKeyDown(sf::Keyboard::Down))
                    {
                        stateGame1.menu++;
                    }
                    if (engine.getKeyDown(sf::Keyboard::Enter))
                    {
                        switch (stateGame1.menu)
                        {
                        case 0:
                            /* code */
                            break;

                        case 1:
                            gameMenu = 2;
                            deltaTime = 99;
                            break;
                        case 2:
                            engine.exit();
                            return 0;
                            break;
                        default:
                            break;
                        }
                    }
                    if (stateGame1.menu < 0)
                        stateGame1.menu = 2;
                    if (stateGame1.menu > 2)
                        stateGame1.menu = 0;

                    switch (stateGame1.menu)
                    {
                    case 0:
                        engine.canvas.text(0.1f, 0.5f, "Iniciar", 28, 0, 0.2f, 0.2f, 1.f);
                        engine.canvas.text(0.1f, 0.6f, "Configurações", 24, 0, 1.f, 1.f, 1.f);
                        engine.canvas.text(0.1f, 0.7f, "Sair", 24, 0, 1.f, 1.f, 1.f);
                        break;
                    case 1:
                        engine.canvas.text(0.1f, 0.5f, "Iniciar", 24, 0, 1.f, 1.f, 1.f);
                        engine.canvas.text(0.1f, 0.6f, "Configurações", 28, 0, 0.2f, 0.2f, 1.f);
                        engine.canvas.text(0.1f, 0.7f, "Sair", 24, 0, 1.f, 1.f, 1.f);
                        break;
                    case 2:
                        engine.canvas.text(0.1f, 0.5f, "Iniciar", 24, 0, 1.f, 1.f, 1.f);
                        engine.canvas.text(0.1f, 0.6f, "Configurações", 24, 0, 1.f, 1.f, 1.f);
                        engine.canvas.text(0.1f, 0.7f, "Sair", 28, 0, 0.2f, 0.2f, 1.f);
                        break;
                    default:
                        break;
                    }
                }
                break;
            case 2: //CONFIGS_1 MENU
                if (deltaTime >= 100 && deltaTime < 200)
                {
                    float move = (deltaTime - 100) / 100.0f;
                    engine.canvas.clearAll();
                    engine.canvas.text(0.1f * move, 0.1f, "Configurações", 36, 0,
                                       move, move, move);
                    engine.canvas.text(0.1f * move, 0.5f, "Vídeo", 24, 0,
                                       move, move, move);
                    engine.canvas.text(0.1f * move, 0.6f, "Saves", 24, 0,
                                       move, move, move);
                    engine.canvas.text(0.1f * move, 0.7f, "Outros", 24, 0,
                                       move, move, move);
                    engine.canvas.text(0.1f * move, 0.8f, "Voltar", 24, 0, move, move, move);
                }
                else if (deltaTime >= 200)
                {

                    engine.canvas.clearAll();
                    engine.canvas.text(0.1f, 0.1f, "Configurações", 36, 0, 1.f, 1.f, 1.f);

                    if (engine.getKeyDown(sf::Keyboard::W) || engine.getKeyDown(sf::Keyboard::Up))
                    {
                        stateGame2.menu--;
                    }
                    if (engine.getKeyDown(sf::Keyboard::S) || engine.getKeyDown(sf::Keyboard::Down))
                    {
                        stateGame2.menu++;
                    }
                    if (engine.getKeyDown(sf::Keyboard::Enter))
                    {
                        switch (stateGame2.menu)
                        {
                        case 0:
                            /* code */
                            break;
                        case 1:
                            /* code */
                            break;
                        case 2:
                            /* code */
                            break;
                        case 3:
                            gameMenu = 1;
                            deltaTime = 99;
                            break;
                        default:
                            break;
                        }
                    }
                    if (stateGame2.menu < 0)
                        stateGame2.menu = 3;
                    if (stateGame2.menu > 3)
                        stateGame2.menu = 0;

                    switch (stateGame2.menu)
                    {
                    case 0:
                        engine.canvas.text(0.1f, 0.5f, "Vídeo", 28, 0, 0.2f, 0.2f, 1.f);
                        engine.canvas.text(0.1f, 0.6f, "Saves", 24, 0, 1.f, 1.f, 1.f);
                        engine.canvas.text(0.1f, 0.7f, "Outros", 24, 0, 1.f, 1.f, 1.f);
                        engine.canvas.text(0.1f, 0.8f, "Voltar", 24, 0, 1.f, 1.f, 1.f);
                        break;
                    case 1:
                        engine.canvas.text(0.1f, 0.5f, "Vídeo", 24, 0, 1.f, 1.f, 1.f);
                        engine.canvas.text(0.1f, 0.6f, "Saves", 28, 0, 0.2f, 0.2f, 1.f);
                        engine.canvas.text(0.1f, 0.7f, "Outros", 24, 0, 1.f, 1.f, 1.f);
                        engine.canvas.text(0.1f, 0.8f, "Voltar", 24, 0, 1.f, 1.f, 1.f);
                        break;
                    case 2:
                        engine.canvas.text(0.1f, 0.5f, "Vídeo", 24, 0, 1.f, 1.f, 1.f);
                        engine.canvas.text(0.1f, 0.6f, "Saves", 24, 0, 1.f, 1.f, 1.f);
                        engine.canvas.text(0.1f, 0.7f, "Outros", 28, 0, 0.2f, 0.2f, 1.f);
                        engine.canvas.text(0.1f, 0.8f, "Voltar", 24, 0, 1.f, 1.f, 1.f);
                        break;
                    case 3:
                        engine.canvas.text(0.1f, 0.5f, "Vídeo", 24, 0, 1.f, 1.f, 1.f);
                        engine.canvas.text(0.1f, 0.6f, "Saves", 24, 0, 1.f, 1.f, 1.f);
                        engine.canvas.text(0.1f, 0.7f, "Outros", 24, 0, 1.f, 1.f, 1.f);
                        engine.canvas.text(0.1f, 0.8f, "Voltar", 28, 0, 0.2f, 0.2f, 1.f);
                        break;
                    default:
                        break;
                    }
                }
                break;
            }

            //THATS A LOT OF IF ELSE, SWITCH!!
            //THIS MAN IS GOING TO BE LIKE YANDERE DEV THIS WAY
            //JUST DO NOT COPY ALL HIS MOVES PLEASE
            //ESPECIALLY THE CRIMINAL MOVES...
    

            nowLoop -= fixedStep;//FINALLY OUTSIDE THE WHILE
        }
        engine.display(); //RENDER ON THE SAME THREAD AS LOGIC, +10000 BRAIN
        //PLEASE, GO READ CLEAN CODE AND GO TO A PSYCHWARD, MYSELF!!1!1
    }
    //THE CODE NEVER GET THERE BY THE WAY...
    engine.exit(); 
    
    return 0;
}
