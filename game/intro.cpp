#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace std;

int intro(Engine &engine)
{
    engine.canvas.gui.clearAll();

    Text titulo;
    titulo.text = "By Matheus Ernesto";
    titulo.name = "titulo1";
    titulo.x = 0.5f;
    titulo.y = 0.5f;
    titulo.align = 1;
    titulo.setColor(0.0f, 0.0f, 0.0f);

    engine.canvas.gui.add(titulo);

    int deltaTime = 0;
    int fade = 0;

    while (!engine.quit())
    {
        
        if (engine.isStepped())
        {
            engine.pollEvents();
            if(deltaTime <= 100){
                float fadein = (float)fade/100.f;
                engine.canvas.gui.getText("titulo1").setColor(fadein,fadein,fadein);
            }else if(deltaTime <= 200){
                float fadeout = (200 - (float)fade)/100;
                engine.canvas.gui.getText("titulo1").setColor(fadeout,fadeout,fadeout);
            }else if(deltaTime == 201){
                fade = 1;
                engine.canvas.gui.getText("titulo1").text = "Metal Himiko";
            }else if(deltaTime <= 300){
                float fadein = (float)fade/100.f;
                engine.canvas.gui.getText("titulo1").setColor(fadein,fadein,fadein);
            }else if(deltaTime <= 400){
                float fadeout = (200 - (float)fade)/100;
                engine.canvas.gui.getText("titulo1").setColor(fadeout,fadeout,fadeout);
            }else{
                return 0;
            }
            deltaTime += 1;
            fade += 1;

            engine.nextStep();
            engine.canvas.update();
            engine.canvas3d.update();
        }
        engine.display();
    }
    return 0;
};
