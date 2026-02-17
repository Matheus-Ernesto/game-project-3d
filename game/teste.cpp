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

        engine.render();
    }
    return 0;
};
