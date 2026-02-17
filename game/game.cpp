#include <iostream>           
#include <SFML/Graphics.hpp>    
#include <SFML/Window.hpp>

#include "../engine/2dCollections.cpp"
#include "../engine/3dCollections.cpp"
#include "../engine/canvas.cpp"
#include "../engine/canvas3d.cpp"
#include "../engine/engine.cpp"
#include "../engine/configsCollections.cpp"

using namespace std;

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
