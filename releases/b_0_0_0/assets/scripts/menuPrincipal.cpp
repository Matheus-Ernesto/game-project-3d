#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <string>
#include <memory>

using namespace std;

// Helper function to add text objects (copied from configuracoes.cpp)
void addText(Engine& engine, const string& name, const string& text, float x, float y, int fontSize, int align, float r, float g, float b, float a = 1.0f) {
    Collections2d::Object obj;
    obj.name = name;
    obj.mesh = new Collections2d::Text();  // raw pointer, will be managed by Canvas
    auto* textMesh = dynamic_cast<Collections2d::Text*>(obj.mesh);
    textMesh->text = text;
    textMesh->fontSize = fontSize;
    textMesh->pathFont = "fonts/arial.ttf"; // adjust as needed
    obj.transform.position = {x, y, 0.f};
    obj.transform.align = align;
    obj.texture.color = {r, g, b, a};
    engine.c2d.gui.add(obj);
}

int menuPrincipal(Engine &engine)
{
    // Clear existing objects
    engine.c2d.gui.deleteAll();
    engine.c3d.world.scene.clearAll();

    // Add title text
    addText(engine, "titulo", "Metal Himiko", 0.0f, 0.2f, 36, 0, 0.0f, 0.0f, 0.0f);
    addText(engine, "iniciar", "Iniciar", 0.0f, 0.7f, 24, 0, 0.0f, 0.0f, 0.0f);
    addText(engine, "configuracoes", "Configurações", 0.0f, 0.8f, 24, 0, 0.0f, 0.0f, 0.0f);
    addText(engine, "sair", "Sair", 0.0f, 0.9f, 24, 0, 0.0f, 0.0f, 0.0f);

    // Create a cube in 3D world
    Collections3d::Object cube;
    cube.name = "cube";
    cube.mesh = make_unique<Collections3d::Cube>();
    cube.transform.position = {3.f, 0.f, 3.f};
    // optional: set color or texture
    cube.texture = make_unique<Collections3d::Texture>();
    cube.texture->setColor(Collections3d::Texture::WHITEBONE);
    engine.c3d.world.scene.add(std::move(cube)); // move to avoid copy

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
                fadeLeftOnePitch(texts, engine); // assumes this function uses engine.c2d
            }
            else
            {
                // Keyboard navigation
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
                    // Return the selected option (0=Iniciar, 1=Configurações, 2=Sair)
                    return option;
                }
                // Clamp option
                if (option < 0) option = 2;
                if (option > 2) option = 0;

                changeSelectedOption(options, option, engine); // assumes this function uses engine.c2d
            }

            deltaTime += 1;

            // Rotate the cube in 3D
            auto& cubeObj = engine.c3d.world.scene.getObject("cube");
            cubeObj.transform.rotation.x += 1.f;
            cubeObj.transform.rotation.y += 1.f;
            cubeObj.transform.rotation.z += 1.f;

            engine.nextStep();
            engine.c2d.update();  // refresh 2D elements
            engine.c3d.update();  // refresh 3D elements
        }
        engine.display();
    }
    return 0;
}