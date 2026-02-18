#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace std;

int intro(Engine &engine)
{

    engine.canvas3d.enable3D = true;
    Object3D cube;
    cube.name = "cubo";
    engine.canvas3d.world.scene.add(cube);

    Object3D cube2;
    cube2.name = "cubo2";
    cube2.pos_y = -2.f;
    cube2.sca_x = 5.f;
    cube2.sca_z = 5.f;
    cube2.texture.r = 1.0f;
    cube2.texture.g = 1.0f;
    cube2.texture.b = 1.0f;
    engine.canvas3d.world.scene.add(cube2);
    
    int color = 0;

    while (!engine.quit())
    {
        engine.pollEvents();
        if (engine.getKeyDown(sf::Keyboard::Enter))
        {
            return 0;
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
            if (color < 7)
            {
                color += 1;
            }

            Object3D &obj = engine.canvas3d.world.scene.getObject3D("cubo");
            obj.texture.setColor(color);
        }
        if (engine.getKeyDown(sf::Keyboard::T))
        {
            if (color > 0)
            {
                color -= 1;
            }

            Object3D &obj = engine.canvas3d.world.scene.getObject3D("cubo");
            obj.texture.setColor(color);
        }

        engine.display();
    }
    return 0;
};
