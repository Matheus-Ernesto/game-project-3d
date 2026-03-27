#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

using namespace std;

int jogo(Engine &engine)
{
    engine.c2d.gui.deleteAll();
    engine.c3d.world.scene.clearAll();

    // Texto de aviso
    Collections2d::Object aviso;
    aviso.name = "aviso";
    aviso.mesh = new Collections2d::Text(); 
    auto* meshAviso = dynamic_cast<Collections2d::Text*>(aviso.mesh);
    meshAviso->text = "Esc para sair.";
    meshAviso->fontSize = 20;
    aviso.transform.position = {0.1f, 0.1f, 0.f};
    engine.c2d.gui.addObj(aviso);

    // Cubo 1 com textura
    Collections3d::Object cube1;
    cube1.name = "cubo1";
    cube1.mesh = make_shared<Collections3d::Cube>();
    cube1.texture = make_shared<Collections3d::Texture>();
    cube1.texture->setTexture("assets/images/check_uv_map.png", 0);
    engine.c3d.world.scene.add(cube1);

    // Cubo 2 (chão)
    Collections3d::Object cube2;
    cube2.name = "cubo2";
    cube2.mesh = make_shared<Collections3d::Cube>();
    cube2.transform.position = {0.f, -2.f, 0.f};
    cube2.transform.scale = {5.f, 1.f, 5.f};  // escala: largura 5, altura 1, profundidade 5
    cube2.texture = make_shared<Collections3d::Texture>();
    cube2.texture->setColor(Collections3d::Texture::GREY); // ou setTexture se quiser
    engine.c3d.world.scene.add(cube2);

    // Esfera
    Collections3d::Object sphere;
    sphere.name = "esfera1";
    sphere.mesh = make_shared<Collections3d::Sphere>();
    auto* sphereMesh = dynamic_cast<Collections3d::Sphere*>(sphere.mesh.get());
    sphereMesh->setResolution(32); // opcional
    sphere.transform.position = {3.f, 0.f, 0.f};
    sphere.texture = make_shared<Collections3d::Texture>();
    sphere.texture->setColor(Collections3d::Texture::BLUE);
    engine.c3d.world.scene.add(sphere);

    // Pré-carregar textura (opcional, mas mantido para compatibilidade)
    engine.c3d.loadTextureToGL("assets/images/check_uv_map.png", 0);

    int deltaTime = 0;

    engine.mouseVisible(false);
    engine.setMousePosition(0.5f, 0.5f);

    Vec2i actualMousePos(engine.mousePosition.x, engine.mousePosition.y);
    Vec2i lastMousePos(engine.mousePosition.x, engine.mousePosition.y);
    Vec2i deltaPos = actualMousePos - lastMousePos;

    float sensibility = 1.f;

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
            float radY = engine.c3d.world.camera.rotationY * M_PI / 180.0f;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                engine.c3d.world.camera.x -= sin(radY) * speed;
                engine.c3d.world.camera.z += cos(radY) * speed;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                engine.c3d.world.camera.x += sin(radY) * speed;
                engine.c3d.world.camera.z -= cos(radY) * speed;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                engine.c3d.world.camera.x += cos(radY) * speed;
                engine.c3d.world.camera.z += sin(radY) * speed;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                engine.c3d.world.camera.x -= cos(radY) * speed;
                engine.c3d.world.camera.z -= sin(radY) * speed;
            }

            if (engine.getKey(sf::Keyboard::Space))
            {
                engine.c3d.world.camera.y += speed;
            }
            if (engine.getKey(sf::Keyboard::LShift))
            {
                engine.c3d.world.camera.y -= speed;
            }

            // Mouse - rotação
            actualMousePos.x = engine.mousePosition.x;
            actualMousePos.y = engine.mousePosition.y;
            deltaPos = actualMousePos - lastMousePos;

            engine.c3d.world.camera.rotationY += deltaPos.x * sensibility;
            engine.c3d.world.camera.rotationX += deltaPos.y * sensibility;

            lastMousePos = actualMousePos;

            auto normalizeAngle = [](float &angle)
            {
                angle = fmod(angle, 360.0f);
                if (angle < 0) angle += 360.0f;
            };
            normalizeAngle(engine.c3d.world.camera.rotationX);
            normalizeAngle(engine.c3d.world.camera.rotationY);
            normalizeAngle(engine.c3d.world.camera.rotationZ);

            // Rotaciona o cubo 1
            auto& cubo1 = engine.c3d.world.scene.getObject("cubo1");
            cubo1.transform.rotation.x += 1.f;
            cubo1.transform.rotation.y += 1.f;
            cubo1.transform.rotation.z += 1.f;

            deltaTime += 1;

            engine.nextStep();
            engine.c2d.update();
            engine.c3d.update();
        }

        engine.display();
    }

    engine.mouseVisible(true);
    return 0;
}