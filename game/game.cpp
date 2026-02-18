#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "../engine/2dCollections.cpp"
#include "../engine/3dCollections.cpp"
#include "../engine/canvas.cpp"
#include "../engine/canvas3d.cpp"
#include "../engine/engine.cpp"
#include "../engine/configsCollections.cpp"

#include "intro.cpp"

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

    if(intro(engine) == 0){
        menuPrincipal(engine);
    }

    engine.exit();
    return 0;
};
