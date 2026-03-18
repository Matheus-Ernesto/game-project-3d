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
    titulo.fontSize = 72;
    titulo.pathFont = "fonts/OstrichSans-Medium.otf";
    titulo.setColor(0.0f, 0.0f, 0.0f);

    engine.canvas.loadTexture("assets/sfml.png");
    engine.canvas.loadTexture("assets/open_gl.png");

    engine.canvas.gui.add(titulo);

    Image opengl;
    opengl.align = 1; // CENTRO
    opengl.name = "opengl";
    opengl.path = "assets/open_gl.png";
    opengl.transparency = 0.f; // 0 = totalmente transparente? (ou você quis dizer 255?)
    opengl.y = 0.3f;

    // Configuração da textura (usar a imagem inteira)
    opengl.x1 = 0; // Começa no pixel 0,0
    opengl.y1 = 0;
    opengl.x2 = 348; // Largura total da imagem
    opengl.y2 = 145; // Altura total da imagem

    // Tamanho na tela (mesmo tamanho da textura, mas você pode ajustar)
    opengl.width = 300;
    opengl.height = 120;

    engine.canvas.gui.add(opengl);

    // SFML - 1001x304 pixels
    Image sfml;
    sfml.align = 1; // CENTRO
    sfml.name = "sfml";
    sfml.path = "assets/sfml.png";
    sfml.transparency = 0.f;
    sfml.y = 0.6f;

    // Configuração da textura (usar a imagem inteira)
    sfml.x1 = 0; // Começa no pixel 0,0
    sfml.y1 = 0;
    sfml.x2 = 1001; // Largura total da imagem
    sfml.y2 = 304;  // Altura total da imagem

    sfml.width = 300;
    sfml.height = 90;

    engine.canvas.gui.add(sfml);

    int deltaTime = 0;
    int fade = 0;

    while (!engine.quit())
    {

        if (engine.isStepped())
        {
            engine.pollEvents();
            if (deltaTime <= 100)
            {
                float fadein = (float)fade / 100.f;
                engine.canvas.gui.getText("titulo1").setColor(fadein, fadein, fadein);
            }
            else if (deltaTime <= 200)
            {
                float fadeout = (200 - (float)fade) / 100;
                engine.canvas.gui.getText("titulo1").setColor(fadeout, fadeout, fadeout);
            }
            else if (deltaTime == 201)
            {
                fade = 1;
                engine.canvas.gui.getText("titulo1").text = "Metal Himiko";
            }
            else if (deltaTime <= 300)
            {
                float fadein = (float)fade / 100.f;
                // engine.canvas.gui.getText("titulo1").setColor(fadein,fadein,fadein);
                engine.canvas.gui.getImage("opengl").transparency = fadein * 255;
                engine.canvas.gui.getImage("sfml").transparency = fadein * 255;
            }
            else if (deltaTime <= 400)
            {
                float fadeout = (200 - (float)fade) / 100;
                // engine.canvas.gui.getText("titulo1").setColor(fadeout,fadeout,fadeout);
                engine.canvas.gui.getImage("opengl").transparency = fadeout * 255;
                engine.canvas.gui.getImage("sfml").transparency = fadeout * 255;
            }
            else
            {
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
