#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace std;

int jogo(Engine &engine)
{
    engine.canvas.gui.clearAll();
    engine.canvas3d.world.scene.clearAll();

    Text aviso;
    aviso.text = "Esc sai do jogo";
    aviso.name = "aviso";
    aviso.x = 0.1f;
    aviso.y = 0.1f;
    engine.canvas.gui.add(aviso);

    int deltaTime = 0;

    while (!engine.quit())
    {
        
        if (engine.isStepped())
        {
            engine.pollEvents();
            if (engine.getKeyDown(sf::Keyboard::Escape))
            {
                return 0;
            }

            deltaTime += 1;
            
            engine.nextStep();
            engine.canvas.update();
            engine.canvas3d.update();
        }
        engine.display();
    }
    return 0;
};
