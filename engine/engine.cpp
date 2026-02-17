// changes: glut -> glfw -> sfml. Ufff
// 30 hours.

// YES, IS NOT IN ENGLISH THE GAME, JUST THE CODE.
// SOMEDAY I GET A LOCALIZATION SCRIPT
// BUT IS NOT TODAY.

// SEE PLANS.MD TO READ WHERE I LOSE MY MIND

#include <unordered_set>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

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
#include <optional>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include <GL/glu.h>
#include <GL/gl.h>

using namespace std;

// BASIC CONFIGS FOR ENGINE
struct Configs
{
    int widthWindow = 800;
    int heightWindow = 600;
    int audioVolume = 10;
    bool fullscreen = false;
    bool showFPS = true;

    bool operator==(const Configs &other) const
    {
        return widthWindow == other.widthWindow &&
               heightWindow == other.heightWindow &&
               fullscreen == other.fullscreen &&
               showFPS == other.showFPS &&
               audioVolume == other.audioVolume;
    }
};

// CONFIG LOADER FOR CONFIG.TXT
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
        file << config.audioVolume << "\n";

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
        file >> config.audioVolume;

        file.close();
        return config;
    }
};

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

class Model
{
};

#include <vector>
using namespace std;

class Texture
{
public:
    float r = 1.f, g = 1.f, b = 1.f, a = 1.f;

    static constexpr int WHITEBONE = 0;
    static constexpr int GREY = 1;
    static constexpr int RED = 2;
    static constexpr int BLUE = 3;
    static constexpr int GREEN = 4;
    static constexpr int GLASSBLUE = 5;
    static constexpr int OTHER = 6;

    void setColor(int type)
    {

        switch (type)
        {
        case WHITEBONE:
            r = 0.8f;
            g = 0.8f;
            b = 0.9f;
            a = 1.f;
            break;

        case GREY:
            r = 0.2f;
            g = 0.2f;
            b = 0.2f;
            a = 1.f;
            break;

        case RED:
            r = 1.f;
            g = 0.f;
            b = 0.f;
            a = 1.f;
            break;

        case BLUE:
            r = 0.f;
            g = 0.f;
            b = 1.f;
            a = 1.f;
            break;

        case GREEN:
            r = 0.f;
            g = 1.f;
            b = 0.f;
            a = 1.f;
            break;

        case GLASSBLUE:
            r = 0.2f;
            g = 0.6f;
            b = 1.f;
            a = 0.5f;
            break;

        case OTHER:
            r = 0.5f;
            g = 0.5f;
            b = 0.5f;
            a = 1.f;
            break;

        default:
            r = 1.f;
            g = 1.f;
            b = 1.f;
            a = 1.f;
            break;
        }
    }

    vector<float> getColor() const
    {
        return {r, g, b, a};
    }
};

class Object3D
{
public:
    float pos_x = 0.f, pos_y = 0.f, pos_z = 0.f;
    float rot_x = 0.f, rot_y = 0.f, rot_z = 0.f;
    float sca_x = 1.f, sca_y = 1.f, sca_z = 1.f;

    Texture texture;
    bool visible = true;
    string name = "";

    Model model;

    int type = 0;
    static constexpr int CUBE = 0;
    static constexpr int MODEL = 1;

    void draw()
    {
        GLfloat mat_diffuse[] = {texture.r, texture.g, texture.b, texture.a};
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

        GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f}; 
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);

        GLfloat mat_shininess[] = {20.0f}; // Brilho
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
        switch (type)
        {
        case CUBE:
            glPushMatrix();
            glTranslatef(pos_x, pos_y, pos_z);

            glRotatef(rot_x, 1.f, 0.f, 0.f);
            glRotatef(rot_y, 0.f, 1.f, 0.f);
            glRotatef(rot_z, 0.f, 0.f, 1.f);

            glScalef(sca_x, sca_y, sca_z);

            glBegin(GL_QUADS);

            glColor4f(texture.r, texture.g, texture.b, texture.a);

            glNormal3f(0.f, 0.f, 1.f);
            glVertex3f(-1.f, -1.f, 1.f);
            glVertex3f(1.f, -1.f, 1.f);
            glVertex3f(1.f, -1.f, -1.f);
            glVertex3f(-1.f, -1.f, -1.f);

            /*

            glVertex3f(-1.f, -1.f, -1.f);
            glVertex3f(1.f, -1.f, -1.f);
            glVertex3f(1.f, 1.f, -1.f);
            glVertex3f(-1.f, 1.f, -1.f);

            glVertex3f(-1.f, -1.f, 1.f);
            glVertex3f(-1.f, 1.f, 1.f);
            glVertex3f(1.f, 1.f, 1.f);
            glVertex3f(1.f, -1.f, 1.f);

            glVertex3f(1.f, -1.f, -1.f);
            glVertex3f(1.f, 1.f, -1.f);
            glVertex3f(1.f, 1.f, 1.f);
            glVertex3f(1.f, -1.f, 1.f);

            glVertex3f(-1.f, 1.f, -1.f);
            glVertex3f(-1.f, 1.f, 1.f);
            glVertex3f(1.f, 1.f, 1.f);
            glVertex3f(1.f, 1.f, -1.f);

            glVertex3f(-1.f, -1.f, -1.f);
            glVertex3f(-1.f, -1.f, 1.f);
            glVertex3f(1.f, -1.f, 1.f);
            glVertex3f(1.f, -1.f, -1.f);

            */

            glEnd();

            glPopMatrix();
            break;
        case MODEL:
            break;
        default:
            break;
        }
    }
};

class Camera
{
public:
    float rotationX, rotationY, rotationZ;
    float x, y, z;
    float fov, limitNear, limitFar;
};

class Scene
{
public:
    vector<Object3D> objects3d;

    void add(Object3D obj)
    {
        objects3d.push_back(obj);
    }

    void draw()
    {
        glEnable(GL_LIGHTING); // Habilita o sistema de iluminação
        glEnable(GL_LIGHT0);   // Habilita a luz 0

        GLfloat light_position[] = {0.0f, 3.0f, 2.0f, 1.0f}; // Posição da luz
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);

        GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f}; // Cor da luz (branca)
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

        GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f}; // Reflexão especular
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

        GLfloat light_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f}; // Luz ambiente (suave)
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);

        for (auto &obj : objects3d)
        {
            if (obj.visible)
                obj.draw();
        }
    }

    Object3D &getObject3D(string name)
    {
        for (auto &obj : objects3d)
        {
            if (obj.name == name)
                return obj;
        }
        static Object3D invalidObject;
        cout << "err Obj " << name << "not found\n";
        return invalidObject;
    }
};

class World
{
public:
    float colorBackgroud_r, colorBackgroud_g, colorBackgroud_b;
    Scene scene;

    Camera camera;

    World()
    {
        camera.x = 0.f;
        camera.y = 0.f;
        camera.z = -10.f;
        camera.rotationX = 60.f;
        camera.rotationY = 30.f;
        camera.rotationZ = 0.f;
        camera.fov = 70.f;
        camera.limitNear = 0.1f;
        camera.limitFar = 1000.f;
        colorBackgroud_r = 0.f;
        colorBackgroud_g = 0.f;
        colorBackgroud_b = 0.f;
    }

    void setBackgroundColor(float r, float g, float b)
    {
        colorBackgroud_r = r;
        colorBackgroud_g = g;
        colorBackgroud_b = b;
    }

    void drawBackground()
    {
        glClearColor(colorBackgroud_r, colorBackgroud_g, colorBackgroud_b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void draw()
    {
        scene.draw();
    };
};

class Canvas3D
{
public:
    World world;
    bool enable3D;

    void draw(sf::RenderWindow &window)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(world.camera.fov, window.getSize().x / window.getSize().y, world.camera.limitNear, world.camera.limitFar);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glTranslatef(world.camera.x, world.camera.y, world.camera.z);
        glRotatef(world.camera.rotationX, 1.f, 0.f, 0.f);
        glRotatef(world.camera.rotationY, 0.f, 1.f, 0.f);
        glRotatef(world.camera.rotationZ, 0.f, 0.f, 1.f);

        world.drawBackground();
        world.draw();
    };

    void apply(sf::RenderWindow &window, int width, int height, bool showFps)
    {
        draw(window);
    };

    void setup(sf::RenderWindow &window)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
    }
};

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

// GAME.CPP

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

    // CONFIG TO ENGINE

    engine.fullscreen = configs.fullscreen;
    engine.width = configs.widthWindow;
    engine.height = configs.heightWindow;
    engine.showFPS = configs.showFPS;

    // Game configs
    engine.timeGameStep = 10; // 60FPS for game steps
    engine.windowName = "Metal Himiko - V0.0";

    engine.start();

    // DUAL CORE??? NEVER HEARD OF THAT, JUST PUT A TIMER ON ONE CORE AND EXECUTE
    // ALL CODE WITH JUST ONE READER, ANYWAYS NO ONE LIKES ATOMIC VARIABLES RIGHT?
    // MAYBE LATER I TEST TO PUT THE RENDER ON OTHER CORE, BUT IM AFRAID OF
    // ACESS SYNC ERROR.

    using clock = std::chrono::steady_clock;

    auto lastLoop = clock::now();
    auto nowLoop = std::chrono::milliseconds(0);
    const auto fixedStep = std::chrono::milliseconds(10);

    // SAVING STATE ON VARIABLES TO NOT RESTART ON LOOP, BUT I PREFER TO NOT USE A LOOP

    struct u_stateGame1 // Game Menu Principal
    {
        int menu = 0;
    };

    struct u_stateGame2 // IS NOT EVEN THE SAME!!!11!! UHUU //Configs
    {
        int menu = 0;
        int resolutionSelect = 0;
        int audioVolume = 0;
        bool fullscreen = false;
    };

    struct u_stateGame3 // IS NOT EVEN THE SAME!!!11!! UHUU ///Video
    {
        int menu = 0;
        int resolutionWidth = 800;
        int resolutionHeight = 600;
        bool ShowFPS = true;
        int volume = 10;
        bool fullscreen = false;
    };
    struct u_stateGame10 // IS NOT EVEN THE SAME!!!11!! UHUU ///Video
    {
        int color = 0;
    };

    u_stateGame1 stateGame1;
    u_stateGame2 stateGame2;
    u_stateGame3 stateGame3;
    u_stateGame10 stateGame10;

    stateGame3.fullscreen = configs.fullscreen;
    stateGame3.resolutionHeight = configs.heightWindow;
    stateGame3.resolutionWidth = configs.widthWindow;
    stateGame3.ShowFPS = configs.showFPS;
    stateGame3.volume = configs.audioVolume;

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
            deltaTime++; // UN1TY COPY???

            // test gui
            switch (gameMenu)
            {
            case 0:
                engine.canvas.clearAll();
                engine.canvas.text(0.5f, 0.5f, "By Matheus Ernesto", 48, 1,
                                   0.f, 0.f, 0.f);
                gameMenu++;
                deltaTime = 0;
                break;
            case 1:
                if (deltaTime <= 80)
                {
                    float fadeIn = deltaTime / 80.0f;
                    engine.canvas.texts[0].setFillColor(sf::Color(fadeIn * 255, fadeIn * 255, fadeIn * 255, 255));
                }
                else if (deltaTime > 120 && deltaTime < 200)
                {
                    float fadeOut = abs(deltaTime - 200) / 80.0f;
                    engine.canvas.texts[0].setFillColor(sf::Color(fadeOut * 255, fadeOut * 255, fadeOut * 255, 255));
                }
                else if (deltaTime == 220)
                {
                    engine.canvas.clearAll();
                    engine.canvas.text(0.5f, 0.5f, "Metal Himiko", 48, 1,
                                       0.f, 0.f, 0.f);
                }
                else if (deltaTime > 220 && deltaTime < 300)
                {
                    float fadeIn = abs(deltaTime - 220) / 80.0f;
                    engine.canvas.texts[0].setFillColor(sf::Color(fadeIn * 255, fadeIn * 255, fadeIn * 255, 1.f * 255));
                }
                else if (deltaTime > 320 && deltaTime < 400)
                {
                    float fadeOut = abs(deltaTime - 400) / 80.0f;
                    engine.canvas.texts[0].setFillColor(sf::Color(fadeOut * 255, fadeOut * 255, fadeOut * 255, 1.f * 255));
                }
                else if (deltaTime > 400)
                {
                    engine.canvas.clearAll();
                    gameMenu++;
                    deltaTime = 0;
                }
                break;
            case 2:
                if (deltaTime == 100)
                {
                    engine.canvas.clearAll();
                    engine.canvas.text(0.f, 0.1f, "Metal Himiko", 36, 0,
                                       0.f, 0.f, 0.f);
                    engine.canvas.text(0.f, 0.5f, "Iniciar", 24, 0,
                                       0.f, 0.f, 0.f);
                    engine.canvas.text(0.f, 0.6f, "Configurações", 24, 0,
                                       0.f, 0.f, 0.f);
                    engine.canvas.text(0.f, 0.7f, "Sair", 24, 0, 0.f, 0.f, 0.f);
                }
                else if (deltaTime > 100 && deltaTime <= 200)
                {
                    float move = abs(deltaTime - 100) / 100.0f;
                    for (sf::Text &text : engine.canvas.texts)
                    {
                        text.setFillColor(sf::Color(move * 255, move * 255, move * 255, 255));
                        text.setPosition(0.1f * move, text.getPosition().y);
                    }
                }
                else if (deltaTime > 200)
                {
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
                            gameMenu = 10;
                            deltaTime = 0;
                            break;

                        case 1:
                            gameMenu = 3;
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
                        engine.canvas.texts[1].setCharacterSize(28);
                        engine.canvas.texts[1].setFillColor(sf::Color(0.2f * 255, 0.2f * 255, 255, 255));
                        engine.canvas.texts[2].setCharacterSize(24);
                        engine.canvas.texts[2].setFillColor(sf::Color(255, 255, 255, 255));
                        engine.canvas.texts[3].setCharacterSize(24);
                        engine.canvas.texts[3].setFillColor(sf::Color(255, 255, 255, 255));
                        break;
                    case 1:
                        engine.canvas.texts[1].setCharacterSize(24);
                        engine.canvas.texts[1].setFillColor(sf::Color(255, 255, 255, 255));
                        engine.canvas.texts[2].setCharacterSize(28);
                        engine.canvas.texts[2].setFillColor(sf::Color(0.2f * 255, 0.2f * 255, 255, 255));
                        engine.canvas.texts[3].setCharacterSize(24);
                        engine.canvas.texts[3].setFillColor(sf::Color(255, 255, 255, 255));
                        break;
                    case 2:
                        engine.canvas.texts[1].setCharacterSize(24);
                        engine.canvas.texts[1].setFillColor(sf::Color(255, 255, 255, 255));
                        engine.canvas.texts[2].setCharacterSize(24);
                        engine.canvas.texts[2].setFillColor(sf::Color(255, 255, 255, 255));
                        engine.canvas.texts[3].setCharacterSize(28);
                        engine.canvas.texts[3].setFillColor(sf::Color(0.2f * 255, 0.2f * 255, 255, 255));
                        break;
                    default:
                        break;
                    }
                }
                break;
            case 3:
                if (deltaTime == 100)
                {
                    engine.canvas.clearAll();
                    engine.canvas.text(0.f, 0.1f, "Configurações", 36, 0,
                                       0.f, 0.f, 0.f);
                    engine.canvas.text(0.f, 0.5f, "Vídeo", 24, 0,
                                       0.f, 0.f, 0.f);
                    engine.canvas.text(0.f, 0.6f, "Saves", 24, 0,
                                       0.f, 0.f, 0.f);
                    engine.canvas.text(0.f, 0.7f, "Outros", 24, 0,
                                       0.f, 0.f, 0.f);
                    engine.canvas.text(0.f, 0.8f, "Voltar", 24, 0, 0.f, 0.f, 0.f);
                }
                if (deltaTime > 100 && deltaTime < 200)
                {
                    float move = abs(deltaTime - 100) / 100.0f;
                    for (sf::Text &text : engine.canvas.texts)
                    {
                        text.setFillColor(sf::Color(move * 255, move * 255, move * 255, 255));
                        text.setPosition(0.1f * move, text.getPosition().y);
                    }
                }
                else if (deltaTime >= 200)
                {
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
                            // Video
                            engine.canvas.clearAll();
                            gameMenu = 4;
                            deltaTime = 99;
                            break;
                        case 1:
                            // Saves
                            engine.canvas.clearAll();
                            gameMenu = 5;
                            deltaTime = 99;
                            break;
                        case 2:
                            // Outros
                            engine.canvas.clearAll();
                            gameMenu = 6;
                            deltaTime = 99;
                            break;
                        case 3:
                            engine.canvas.clearAll();
                            gameMenu = 2;
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
                        engine.canvas.texts[1].setCharacterSize(28);
                        engine.canvas.texts[1].setFillColor(sf::Color(0.2f * 255, 0.2f * 255, 255, 255));
                        engine.canvas.texts[2].setCharacterSize(24);
                        engine.canvas.texts[2].setFillColor(sf::Color(255, 255, 255, 255));
                        engine.canvas.texts[3].setCharacterSize(24);
                        engine.canvas.texts[3].setFillColor(sf::Color(255, 255, 255, 255));
                        engine.canvas.texts[4].setCharacterSize(24);
                        engine.canvas.texts[4].setFillColor(sf::Color(255, 255, 255, 255));
                        break;
                    case 1:
                        engine.canvas.texts[1].setCharacterSize(24);
                        engine.canvas.texts[1].setFillColor(sf::Color(255, 255, 255, 255));
                        engine.canvas.texts[2].setCharacterSize(28);
                        engine.canvas.texts[2].setFillColor(sf::Color(0.2f * 255, 0.2f * 255, 255, 255));
                        engine.canvas.texts[3].setCharacterSize(24);
                        engine.canvas.texts[3].setFillColor(sf::Color(255, 255, 255, 255));
                        engine.canvas.texts[4].setCharacterSize(24);
                        engine.canvas.texts[4].setFillColor(sf::Color(255, 255, 255, 255));
                        break;
                    case 2:
                        engine.canvas.texts[1].setCharacterSize(24);
                        engine.canvas.texts[1].setFillColor(sf::Color(255, 255, 255, 255));
                        engine.canvas.texts[2].setCharacterSize(24);
                        engine.canvas.texts[2].setFillColor(sf::Color(255, 255, 255, 255));
                        engine.canvas.texts[3].setCharacterSize(28);
                        engine.canvas.texts[3].setFillColor(sf::Color(0.2f * 255, 0.2f * 255, 255, 255));
                        engine.canvas.texts[4].setCharacterSize(24);
                        engine.canvas.texts[4].setFillColor(sf::Color(255, 255, 255, 255));
                        break;
                    case 3:
                        engine.canvas.texts[1].setCharacterSize(24);
                        engine.canvas.texts[1].setFillColor(sf::Color(255, 255, 255, 255));
                        engine.canvas.texts[2].setCharacterSize(24);
                        engine.canvas.texts[2].setFillColor(sf::Color(255, 255, 255, 255));
                        engine.canvas.texts[3].setCharacterSize(24);
                        engine.canvas.texts[3].setFillColor(sf::Color(255, 255, 255, 255));
                        engine.canvas.texts[4].setCharacterSize(28);
                        engine.canvas.texts[4].setFillColor(sf::Color(0.2f * 255, 0.2f * 255, 255, 255));
                        break;
                    default:
                        break;
                    }
                }
                break;
            case 4:
                if (deltaTime == 100)
                {
                    engine.canvas.clearAll();
                    engine.canvas.text(0.f, 0.1f, "Vídeo", 36, 0,
                                       0.f, 0.f, 0.f);
                    engine.canvas.text(0.f, 0.5f, "Resolução:", 24, 0,
                                       0.f, 0.f, 0.f);
                    engine.canvas.text(0.f, 0.6f, "Tela Cheia:", 24, 0,
                                       0.f, 0.f, 0.f);
                    engine.canvas.text(0.f, 0.7f, "Mostrar FPS: ", 24, 0,
                                       0.f, 0.f, 0.f);
                    engine.canvas.text(0.f, 0.8f, "Voltar/Aplicar", 24, 0, 0.f, 0.f, 0.f);
                }
                else if (deltaTime > 100 && deltaTime < 200)
                {
                    float move = abs(deltaTime - 100) / 100.0f;
                    for (sf::Text &text : engine.canvas.texts)
                    {
                        text.setFillColor(sf::Color(move * 255, move * 255, move * 255, 255));
                        text.setPosition(0.1f * move, text.getPosition().y);
                    }
                }
                else if (deltaTime == 200)
                {
                    engine.canvas.text(0.5f, 0.5f, "", 28, 0,
                                       0.2f, 0.2f, 1.f); // resolution
                    engine.canvas.text(0.5f, 0.6f, "", 28, 0,
                                       0.2f, 0.2f, 1.f); // fullscreen
                    engine.canvas.text(0.5f, 0.7f, "", 28, 0,
                                       0.2f, 0.2f, 1.f); // fps
                }
                else if (deltaTime > 200)
                {
                    if (engine.getKeyDown(sf::Keyboard::W) || engine.getKeyDown(sf::Keyboard::Up))
                    {
                        stateGame3.menu--;
                    }
                    if (engine.getKeyDown(sf::Keyboard::S) || engine.getKeyDown(sf::Keyboard::Down))
                    {
                        stateGame3.menu++;
                    }
                    if (engine.getKeyDown(sf::Keyboard::Enter))
                    {
                        switch (stateGame3.menu)
                        {
                        case 0:
                            // Resolução
                            switch (stateGame3.resolutionWidth)
                            {
                            case 640:
                                stateGame3.resolutionWidth = 800;
                                stateGame3.resolutionHeight = 600;
                                break;
                            case 800:
                                stateGame3.resolutionWidth = 1024;
                                stateGame3.resolutionHeight = 576;
                                break;
                            case 1024:
                                stateGame3.resolutionWidth = 1280;
                                stateGame3.resolutionHeight = 720;
                                break;
                            case 1280:
                                stateGame3.resolutionWidth = 1366;
                                stateGame3.resolutionHeight = 768;
                                break;
                            case 1366:
                                stateGame3.resolutionWidth = 1600;
                                stateGame3.resolutionHeight = 900;
                                break;
                            case 1600:
                                stateGame3.resolutionWidth = 1920;
                                stateGame3.resolutionHeight = 1080;
                                break;
                            case 1920:
                                stateGame3.resolutionWidth = 2560;
                                stateGame3.resolutionHeight = 1440;
                                break;
                            case 2560:
                                stateGame3.resolutionWidth = 640;
                                stateGame3.resolutionHeight = 480;
                                break;
                            default:
                                break;
                            }
                            break;
                        case 1:
                            // Tela Cheia
                            stateGame3.fullscreen = stateGame3.fullscreen ? false : true;
                            break;
                        case 2:
                            // Mostrar FPS
                            stateGame3.ShowFPS = stateGame3.ShowFPS ? false : true;
                            break;
                        case 3:
                            gameMenu = 7;
                            deltaTime = 0;
                            break;
                        default:
                            break;
                        }
                    }
                    if (stateGame3.menu < 0)
                        stateGame3.menu = 3;
                    if (stateGame3.menu > 3)
                        stateGame3.menu = 0;

                    switch (stateGame3.menu)
                    {
                    case 0:
                    {
                        engine.canvas.texts[1].setCharacterSize(28);
                        engine.canvas.texts[1].setFillColor(sf::Color(0.2f * 255, 0.2f * 255, 255, 255));
                        string resolution = (to_string(stateGame3.resolutionWidth) + "x" + to_string(stateGame3.resolutionHeight)).c_str();
                        sf::String resolutionText = sf::String::fromUtf8(resolution.begin(), resolution.end());
                        engine.canvas.texts[5].setString(resolutionText);
                        engine.canvas.texts[2].setCharacterSize(24);
                        engine.canvas.texts[2].setFillColor(sf::Color(255, 255, 255, 255));
                        engine.canvas.texts[3].setCharacterSize(24);
                        engine.canvas.texts[3].setFillColor(sf::Color(255, 255, 255, 255));
                        engine.canvas.texts[4].setCharacterSize(24);
                        engine.canvas.texts[4].setFillColor(sf::Color(255, 255, 255, 255));

                        engine.canvas.texts[6].setString("");
                        engine.canvas.texts[7].setString("");
                        break;
                    }
                    case 1:
                    {
                        engine.canvas.texts[2].setCharacterSize(28);
                        engine.canvas.texts[2].setFillColor(sf::Color(0.2f * 255, 0.2f * 255, 255, 255));
                        string fullscreen = (stateGame3.fullscreen ? "Sim (Tela Cheia Exclusiva)" : "Não (Modo Janela)");
                        sf::String fullscreenText = sf::String::fromUtf8(fullscreen.begin(), fullscreen.end());
                        engine.canvas.texts[6].setString(fullscreenText);
                        engine.canvas.texts[1].setCharacterSize(24);
                        engine.canvas.texts[1].setFillColor(sf::Color(255, 255, 255, 255));
                        engine.canvas.texts[3].setCharacterSize(24);
                        engine.canvas.texts[3].setFillColor(sf::Color(255, 255, 255, 255));
                        engine.canvas.texts[4].setCharacterSize(24);
                        engine.canvas.texts[4].setFillColor(sf::Color(255, 255, 255, 255));

                        engine.canvas.texts[5].setString("");
                        engine.canvas.texts[7].setString("");
                        break;
                    }
                    case 2:
                    {
                        engine.canvas.texts[3].setCharacterSize(28);
                        engine.canvas.texts[3].setFillColor(sf::Color(0.2f * 255, 0.2f * 255, 255, 255));
                        string showFps = (stateGame3.ShowFPS ? "Sim" : "Não");
                        sf::String showFpsText = sf::String::fromUtf8(showFps.begin(), showFps.end());
                        engine.canvas.texts[7].setString(showFpsText);
                        engine.canvas.texts[1].setCharacterSize(24);
                        engine.canvas.texts[1].setFillColor(sf::Color(255, 255, 255, 255));
                        engine.canvas.texts[2].setCharacterSize(24);
                        engine.canvas.texts[2].setFillColor(sf::Color(255, 255, 255, 255));
                        engine.canvas.texts[4].setCharacterSize(24);
                        engine.canvas.texts[4].setFillColor(sf::Color(255, 255, 255, 255));

                        engine.canvas.texts[5].setString("");
                        engine.canvas.texts[6].setString("");
                        break;
                    }
                    case 3:
                    {
                        engine.canvas.texts[4].setCharacterSize(28);
                        engine.canvas.texts[4].setFillColor(sf::Color(0.2f * 255, 0.2f * 255, 255, 255));
                        engine.canvas.texts[2].setCharacterSize(24);
                        engine.canvas.texts[2].setFillColor(sf::Color(255, 255, 255, 255));
                        engine.canvas.texts[3].setCharacterSize(24);
                        engine.canvas.texts[3].setFillColor(sf::Color(255, 255, 255, 255));
                        engine.canvas.texts[1].setCharacterSize(24);
                        engine.canvas.texts[1].setFillColor(sf::Color(255, 255, 255, 255));

                        engine.canvas.texts[5].setString("");
                        engine.canvas.texts[6].setString("");
                        engine.canvas.texts[7].setString("");
                        break;
                    }
                    default:
                        break;
                    }
                }
                break;
            case 5:
                // CONFIGS_1 SAVES
                if (deltaTime == 100)
                {
                    engine.canvas.clearAll();
                    engine.canvas.text(0.f, 0.1f, "Saves", 36, 0,
                                       0.f, 0.f, 0.f);
                    engine.canvas.text(0.f, 0.5f, "Ainda não implementado...", 24, 0,
                                       0.f, 0.f, 0.f);
                    engine.canvas.text(0.f, 0.8f, "Voltar", 24, 0, 0.f, 0.f, 0.f);
                }
                else if (deltaTime > 100 && deltaTime < 200)
                {
                    float move = abs(deltaTime - 100) / 100.0f;
                    for (sf::Text &text : engine.canvas.texts)
                    {
                        text.setFillColor(sf::Color(move * 255, move * 255, move * 255, 255));
                        text.setPosition(0.1f * move, text.getPosition().y);
                    }
                }
                else if (deltaTime == 200)
                {
                    engine.canvas.texts[2].setFillColor(sf::Color(0.2f * 255, 0.2f * 255, 255, 255));
                    engine.canvas.texts[2].setCharacterSize(28);
                }
                else if (deltaTime > 200)
                {
                    if (engine.getKeyDown(sf::Keyboard::Enter))
                    {
                        gameMenu = 3;
                        deltaTime = 99;
                    }
                }
                break;
            case 6:
                // CONFIGS_1 AUDIO
                if (deltaTime == 100)
                {
                    engine.canvas.clearAll();
                    engine.canvas.text(0.f, 0.1f, "Audio", 36, 0,
                                       0.f, 0.f, 0.f);
                    engine.canvas.text(0.f, 0.5f, "Volume:", 24, 0,
                                       0.2f, 0.2f, 1.f);
                    engine.canvas.text(0.f, 0.8f, "Voltar:", 24, 0,
                                       0.f, 0.f, 0.f);
                }

                else if (deltaTime > 100 && deltaTime < 200)
                {
                    float move = abs(deltaTime - 100) / 100.0f;
                    for (sf::Text &text : engine.canvas.texts)
                    {
                        text.setFillColor(sf::Color(move * 255, move * 255, move * 255, 255));
                        text.setPosition(0.1f * move, text.getPosition().y);
                    }
                }
                else if (deltaTime == 200)
                {
                    engine.canvas.text(0.5f, 0.5f, "", 24, 0, 0.2f, 0.2f, 1.f);
                }
                else if (deltaTime > 200)
                {
                    if (engine.getKeyDown(sf::Keyboard::W) || engine.getKeyDown(sf::Keyboard::Up))
                    {
                        stateGame3.menu--;
                    }
                    if (engine.getKeyDown(sf::Keyboard::S) || engine.getKeyDown(sf::Keyboard::Down))
                    {
                        stateGame3.menu++;
                    }
                    if (engine.getKeyDown(sf::Keyboard::D) || engine.getKeyDown(sf::Keyboard::Right))
                    {
                        if (stateGame3.volume < 10)
                            stateGame3.volume++;
                    }
                    if (engine.getKeyDown(sf::Keyboard::A) || engine.getKeyDown(sf::Keyboard::Left))
                    {
                        if (stateGame3.volume > 0)
                            stateGame3.volume--;
                    }
                    if (engine.getKeyDown(sf::Keyboard::Enter))
                    {
                        if (stateGame3.menu == 1)
                        {
                            gameMenu = 3;
                            deltaTime = 99;
                        }
                    }

                    if (stateGame3.menu < 0)
                        stateGame3.menu = 1;
                    if (stateGame3.menu > 1)
                        stateGame3.menu = 0;

                    if (stateGame3.menu == 0)
                    {
                        engine.canvas.texts[1].setCharacterSize(28);
                        engine.canvas.texts[1].setFillColor(sf::Color(0.2f * 255, 0.2f * 255, 255, 255));
                        engine.canvas.texts[2].setCharacterSize(24);
                        engine.canvas.texts[2].setFillColor(sf::Color(255, 255, 255, 255));
                        engine.canvas.texts[3].setString(to_string(stateGame3.volume));
                    }
                    else
                    {
                        engine.canvas.texts[1].setCharacterSize(24);
                        engine.canvas.texts[1].setFillColor(sf::Color(255, 255, 255, 255));
                        engine.canvas.texts[2].setCharacterSize(28);
                        engine.canvas.texts[2].setFillColor(sf::Color(0.2f * 255, 0.2f * 255, 255, 255));
                        engine.canvas.texts[3].setString("");
                    }
                }
                break;
            case 7:
                if (deltaTime == 1)
                {
                    engine.canvas.clearAll();
                    engine.canvas.text(0.5f, 0.5f, "Reinicie o jogo para aplicar.\nCaso algum problema, apague o configs.txt", 24, 1, 1.f, 1.f, 1.f);
                }

                if (engine.getKeyDown(sf::Keyboard::Enter))
                {
                    gameMenu = 3;
                    deltaTime = 99;

                    configs.fullscreen = stateGame3.fullscreen;
                    configs.widthWindow = stateGame3.resolutionWidth;
                    configs.heightWindow = stateGame3.resolutionHeight;
                    configs.showFPS = stateGame3.ShowFPS;

                    loader.saveConfig(configs);
                }
                break;
            case 10:
                if (deltaTime == 1)
                {

                    engine.canvas.clearAll();
                    engine.canvas.text(0.1f, 0.1f, "Jogando... Enter para voltar", 16, 0, 1.f, 1.f, 1.f);
                    engine.canvas.image(0.1f, 0.3f, "assets/check_uv_map.png", 0, 0, 0, 1024, 1024, 100, 100, 128.f);
                    engine.canvas3d.enable3D = true;

                    Object3D cube;
                    cube.name = "cubo";
                    engine.canvas3d.world.scene.add(cube);
                }
                if (engine.getKeyDown(sf::Keyboard::Enter))
                {
                    gameMenu = 2;
                    deltaTime = 99;
                    engine.canvas3d.enable3D = false;
                }
                if (engine.getKeyDown(sf::Keyboard::Up))
                {
                    engine.canvas3d.world.camera.rotationX += 5.f;
                }
                if (engine.getKeyDown(sf::Keyboard::Down))
                {
                    engine.canvas3d.world.camera.rotationX -= 5.f;
                }
                if (engine.getKeyDown(sf::Keyboard::Left))
                {
                    engine.canvas3d.world.camera.rotationY += 5.f;
                }
                if (engine.getKeyDown(sf::Keyboard::Right))
                {
                    engine.canvas3d.world.camera.rotationY -= 5.f;
                }
                if (engine.getKeyDown(sf::Keyboard::W))
                {
                    engine.canvas3d.world.camera.z += 1.f;
                }
                if (engine.getKeyDown(sf::Keyboard::S))
                {
                    engine.canvas3d.world.camera.z -= 1.f;
                }
                if (engine.getKeyDown(sf::Keyboard::A))
                {
                    engine.canvas3d.world.camera.x += 1.f;
                }
                if (engine.getKeyDown(sf::Keyboard::D))
                {
                    engine.canvas3d.world.camera.x -= 1.f;
                }
                if (engine.getKeyDown(sf::Keyboard::Q))
                {
                    engine.canvas3d.world.camera.y += 1.f;
                }
                if (engine.getKeyDown(sf::Keyboard::E))
                {
                    engine.canvas3d.world.camera.y -= 1.f;
                }
                if (engine.getKeyDown(sf::Keyboard::R))
                {
                    if (stateGame10.color < 7)
                    {
                        stateGame10.color += 1;
                    }

                    Object3D &obj = engine.canvas3d.world.scene.getObject3D("cubo");
                    obj.texture.setColor(stateGame10.color);
                }
                if (engine.getKeyDown(sf::Keyboard::T))
                {
                    if (stateGame10.color > 0)
                    {
                        stateGame10.color -= 1;
                    }

                    Object3D &obj = engine.canvas3d.world.scene.getObject3D("cubo");
                    obj.texture.setColor(stateGame10.color);
                }
                break;

            default:
                break;
            }
            // THATS A LOT OF IF ELSE, SWITCH!!
            // THIS MAN IS GOING TO BE LIKE YANDERE DEV THIS WAY
            // JUST DO NOT COPY ALL HIS MOVES PLEASE
            // ESPECIALLY THE CRIMINAL MOVES...

            nowLoop -= fixedStep; // FINALLY OUTSIDE THE WHILE
        } // RENDER ON THE SAME THREAD AS LOGIC, +10000 BRAIN

        engine.display();
        // PLEASE, GO READ CLEAN CODE AND GO TO A PSYCHWARD, MYSELF!!1!1
    }
    // THE CODE NEVER GET THERE BY THE WAY...
    engine.exit();

    return 0;
}
