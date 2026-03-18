#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <string>

using namespace std;

int menuPrincipal(Engine &engine)
{
    engine.canvas.gui.clearAll();
    engine.canvas3d.world.scene.clearAll();

    Text titulo;
    titulo.text = "Metal Himiko";
    titulo.name = "titulo";
    titulo.x = 0.0f;
    titulo.y = 0.2f;
    titulo.fontSize = 36;
    titulo.align = 0;
    titulo.setColor(0.0f, 0.0f, 0.0f);
    engine.canvas.gui.add(titulo);

    Text iniciar;
    iniciar.text = "Iniciar";
    iniciar.name = "iniciar";
    iniciar.x = 0.0f;
    iniciar.y = 0.7f;
    iniciar.align = 0;
    iniciar.setColor(0.0f, 0.0f, 0.0f);
    engine.canvas.gui.add(iniciar);

    Text configuracoes;
    configuracoes.text = "Configurações";
    configuracoes.name = "configuracoes";
    configuracoes.x = 0.0f;
    configuracoes.y = 0.8f;
    configuracoes.align = 0;
    configuracoes.setColor(0.0f, 0.0f, 0.0f);
    engine.canvas.gui.add(configuracoes);

    Text sair;
    sair.text = "Sair";
    sair.name = "sair";
    sair.x = 0.0f;
    sair.y = 0.9f;
    sair.align = 0;
    sair.setColor(0.0f, 0.0f, 0.0f);
    engine.canvas.gui.add(sair);

    Object3D cube;
    cube.name = "cube";
    cube.pos_x = 3.f;
    cube.pos_z = 3.f;

    engine.canvas3d.world.scene.add(cube);

    int deltaTime = 0;

    int option = 0;
    vector<string> texts = {"titulo", "iniciar", "configuracoes", "sair"};
    vector<string> options = {"iniciar", "configuracoes", "sair"};

    while (!engine.quit())
    {
        if (engine.isStepped())
        {
            engine.pollEvents();
            if (deltaTime < 100)
            {
                fadeLeftOnePitch(texts, engine);
            }
            else
            {
                if (engine.getKeyDown(sf::Keyboard::W) || engine.getKeyDown(sf::Keyboard::Up))
                {
                    option--;
                }
                if (engine.getKeyDown(sf::Keyboard::S) || engine.getKeyDown(sf::Keyboard::Down))
                {
                    option++;
                }
                if (engine.getKeyDown(sf::Keyboard::Enter))
                {
                    return option;
                }
                if (option < 0)
                    option = 2;
                if (option > 2)
                    option = 0;

                changeSelectedOption(options, option, engine);
            }

            deltaTime += 1;

            engine.canvas3d.world.scene.getObject3D("cube").rot_x += 1.f;
            engine.canvas3d.world.scene.getObject3D("cube").rot_y += 1.f;
            engine.canvas3d.world.scene.getObject3D("cube").rot_z += 1.f;

            engine.nextStep();
            engine.canvas.update();
            engine.canvas3d.update();
        }
        engine.display();
    }
    return 0;
};
