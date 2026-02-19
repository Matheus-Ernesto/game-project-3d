#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "../engine/2dCollections.cpp"
#include "../engine/3dCollections.cpp"
#include "../engine/canvas.cpp"
#include "../engine/canvas3d.cpp"
#include "../engine/engine.cpp"
#include "../engine/configsCollections.cpp"
#include "utils.cpp"

#include "intro.cpp"
#include "jogo.cpp"
#include "configuracoes.cpp"
#include "menuPrincipal.cpp"

using namespace std;

Engine engine;
Configs configs;

int main()
{
    Loader loader;
    Configs t_configs = loader.loadConfig();
    if (configs == t_configs)
    {
        loader.saveConfig(configs);
    }
    else
    {
        configs = t_configs;
    }

    // CONFIG TO ENGINE

    engine.fullscreen = configs.fullscreen;
    engine.width = configs.widthWindow;
    engine.height = configs.heightWindow;
    engine.showFPS = configs.showFPS;

    // Game configs
    engine.windowName = "Metal Himiko - V0.0";

    engine.setStep(16);
    engine.start();

    engine.nextStep();

    int level = 0;

    //intro(engine);
    while (level >= 0 && !engine.quit())
    {
        switch (menuPrincipal(engine))
        {
        case 0:
            jogo(engine);
            break;
        case 1:
            level = 1;
            while (level == 1 && !engine.quit())
            {
                switch (configuracoes(engine))
                {
                case 0:
                    video(engine, configs);
                    break;
                case 1:
                    outros(engine, configs);
                    break;
                case 2:
                    saves(engine);
                    break;
                default:
                    level = 0;
                    break;
                }
            }
            loader.saveConfig(configs);
            break;
        default:
            level = -1;
            break;
        }
    }
    // sair
    engine.exit();
    return 0;
};
