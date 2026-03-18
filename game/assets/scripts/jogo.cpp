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
    cube.texture.setTexture("assets/check_uv_map.png", 0);
    engine.canvas3d.world.scene.add(cube);

    cube.name = "cubo2";
    cube.pos_y = -2;
    cube.sca_x = 5;
    cube.sca_z = 5;
    engine.canvas3d.world.scene.add(cube);

    cube.name = "esfera1";
    cube.pos_y = 0;
    cube.pos_x = 3;
    cube.sca_x = 1;
    cube.sca_z = 1;
    cube.type = cube.SPHERE_SUB4;
    engine.canvas3d.world.scene.add(cube);

    int texGrass = engine.canvas3d.loadTextureToGL("assets/check_uv_map.png", 0);

    int deltaTime = 0;

    engine.mouseVisible(false);
    engine.setMousePosition(0.5f, 0.5f);

    Vec2i actualMousePos(engine.mousePosition.x, engine.mousePosition.y);
    Vec2i lastMousePos(engine.mousePosition.x, engine.mousePosition.y);

    Vec2i deltaPos = actualMousePos - lastMousePos;

    float sensibility = 0.1f;

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

            float speed = 0.3f;
            float radY = engine.canvas3d.world.camera.rotationY * M_PI / 180.0f;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                engine.canvas3d.world.camera.x -= sin(radY) * speed;
                engine.canvas3d.world.camera.z += cos(radY) * speed;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                engine.canvas3d.world.camera.x += sin(radY) * speed;
                engine.canvas3d.world.camera.z -= cos(radY) * speed;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                engine.canvas3d.world.camera.x += cos(radY) * speed;
                engine.canvas3d.world.camera.z += sin(radY) * speed;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                engine.canvas3d.world.camera.x -= cos(radY) * speed;
                engine.canvas3d.world.camera.z -= sin(radY) * speed;
            }

            if (engine.getKey(sf::Keyboard::LShift))
            {
                engine.canvas3d.world.camera.y += speed;
            }
            if (engine.getKey(sf::Keyboard::Space))
            {
                engine.canvas3d.world.camera.y -= speed;
            }

            // MOUSE - Rotação
            actualMousePos.x = engine.mousePosition.x;
            actualMousePos.y = engine.mousePosition.y;
            deltaPos = actualMousePos - lastMousePos;

            engine.canvas3d.world.camera.rotationY += deltaPos.x;
            engine.canvas3d.world.camera.rotationX += deltaPos.y;

            lastMousePos = actualMousePos;

            auto normalizeAngle = [](float &angle)
            {
                angle = fmod(angle, 360.0f);
                if (angle < 0)
                    angle += 360.0f;
            };

            normalizeAngle(engine.canvas3d.world.camera.rotationX);
            normalizeAngle(engine.canvas3d.world.camera.rotationY);
            normalizeAngle(engine.canvas3d.world.camera.rotationZ);

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
