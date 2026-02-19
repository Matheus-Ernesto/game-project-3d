#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace std;

int jogo(Engine &engine)
{
    engine.canvas.gui.clearAll();
    engine.canvas3d.world.scene.clearAll();

    Text aviso;
    aviso.text = "Esc para sair.";
    aviso.name = "aviso";
    aviso.x = 0.1f;
    aviso.y = 0.1f;
    engine.canvas.gui.add(aviso);

    Object3D cube;
    cube.name = "cubo1";
    engine.canvas3d.world.scene.add(cube);

    cube.name = "cubo2";
    cube.pos_y = -2;
    cube.sca_x = 5;
    cube.sca_z = 5;
    engine.canvas3d.world.scene.add(cube);

    int deltaTime = 0;

    engine.mouseVisible(false);
    engine.setMousePosition(0.5f,0.5f);

    Vec2i actualMousePos(engine.mousePosition.x, engine.mousePosition.y);
    Vec2i lastMousePos(engine.mousePosition.x, engine.mousePosition.y);

    Vec2i deltaPos = actualMousePos - lastMousePos;

    float sensibility = 0.1f;

    Vec2i center(engine.width / 2, engine.height / 2);


    while (!engine.quit())
    {
        if (engine.isStepped())
        {
            engine.pollEvents();
            if (engine.getKeyDown(sf::Keyboard::Escape))
            {
                engine.mouseVisible(true);
                return 0;
            }

            if (engine.getKeyDown(sf::Keyboard::W))
            {
                engine.canvas3d.world.camera.z += 0.5f;
            }
            if (engine.getKeyDown(sf::Keyboard::S))
            {
                engine.canvas3d.world.camera.z -= 0.5f;
            }
            if (engine.getKeyDown(sf::Keyboard::A))
            {
                engine.canvas3d.world.camera.x += 0.5f;
            }
            if (engine.getKeyDown(sf::Keyboard::D))
            {
                engine.canvas3d.world.camera.x -= 0.5f;
            }
            if (engine.getKeyDown(sf::Keyboard::LShift))
            {
                engine.canvas3d.world.camera.y += 0.5f;
            }
            if (engine.getKeyDown(sf::Keyboard::Space))
            {
                engine.canvas3d.world.camera.y -= 0.5f;
            }
            actualMousePos.x = engine.mousePosition.x;
            actualMousePos.y = engine.mousePosition.y;
            deltaPos = actualMousePos - lastMousePos;

            engine.canvas3d.world.camera.rotationY += deltaPos.x;
            engine.canvas3d.world.camera.rotationX += deltaPos.y;

            lastMousePos = actualMousePos;

            // Atualização do objeto cubo
            engine.canvas3d.world.scene.getObject3D("cubo1").rot_x += 1.f;
            engine.canvas3d.world.scene.getObject3D("cubo1").rot_y += 1.f;
            engine.canvas3d.world.scene.getObject3D("cubo1").rot_z += 1.f;
            
            deltaTime += 1;

            engine.nextStep();
            engine.canvas.update();
            engine.canvas3d.update();
        }
        engine.display();
    }
    engine.mouseVisible(true);
    return 0;
};
