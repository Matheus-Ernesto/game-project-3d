#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace std;

int intro(Engine &engine)
{
    engine.canvas.clearAll();

    Text titulo;
    titulo.text = "By Matheus Ernesto";
    titulo.x = 0.5f;
    titulo.y = 0.5f;
    titulo.align = 1;

    titulo.setColor(0.0f, 0.0f, 0.0f);

    Text titulo2 = titulo;
    titulo2.text = "Metal Himiko"

    engine.canvas.add(titulo);

    Object3D cube;
    cube.name = "cube";

    engine.canvas3d.world.scene.add(cube);

    int deltaTime = 0;
    int fade = 0;

    while (!engine.quit())
    {
        engine.pollEvents();
        engine.display();

        if (engine.isStepped())
        {
            if(deltaTime <= 100){
                engine.canvas.texts[0].setFillColor(sf::Color(255,255,255,float(fade)*2.55f));
            }else if(deltaTime <= 200){
                int fadeout = abs(fade - 200);
                engine.canvas.texts[0].setFillColor(sf::Color(255,255,255,float(fadeout)*2.55f));
            }else if(deltaTime == 201){
                fade = 1;
                engine.canvas.texts[0].;
            }else if(deltaTime <= 300){
                engine.canvas.texts[0].setFillColor(sf::Color(255,255,255,float(fade)*2.55f));
            }else if(deltaTime <= 400){
                int fadeout = abs(fade - 200);
                engine.canvas.texts[0].setFillColor(sf::Color(255,255,255,float(fadeout)*2.55f));
            }
            deltaTime += 1;
            fade += 1;

            engine.canvas3d.world.scene.getObject3D("cube").rot_x += 1.f;

            engine.nextStep();
        }
    }
    return 0;
};
