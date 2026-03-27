#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <memory>

using namespace std;

int intro(Engine &engine)
{
    // Limpa tudo
    engine.c2d.gui.deleteAll();
    engine.c3d.world.scene.clearAll();

    Collections2d::Object titulo;
    titulo.name = "titulo1";
    titulo.transform.align = 1;
    titulo.transform.position = {0.5f,0.5f,0.0f};

    auto* meshTitulo = new Collections2d::Text();
    meshTitulo->text = "By Matheus Ernesto";
    meshTitulo->fontSize = 72;
    meshTitulo->pathFont = "fonts/OstrichSans-Medium.otf";

    titulo.mesh = meshTitulo;
    engine.c2d.gui.addObj(titulo);

    engine.c2d.loadTexture("assets/images/sfml.png");
    engine.c2d.loadTexture("assets/images/open_gl.png");

    Collections2d::Object imagem1;
    imagem1.name = "imagem1";
    imagem1.transform.align = 1;
    imagem1.transform.position = {0.5f,0.3f,0.0f};

    auto* meshOpenGL = new Collections2d::Image();
    meshOpenGL->coordinatesEnd = {348.f, 145.f};
    meshOpenGL->path = "assets/images/open_gl.png";
    meshOpenGL->width = 300.f;
    meshOpenGL->height = 120.f;
    meshOpenGL->transparency = 0.f;

    imagem1.mesh = meshOpenGL;
    engine.c2d.gui.addObj(imagem1);

    Collections2d::Object imagem2;
    imagem2.name = "imagem2";
    imagem2.transform.align = 1;
    imagem2.transform.position = {0.5f,0.7f,0.0f};

    auto* meshSfml = new Collections2d::Image();
    meshSfml->coordinatesEnd = {1001.f, 304.f};
    meshSfml->path = "assets/images/sfml.png";
    meshSfml->width = 300.f;
    meshSfml->height = 90.f;
    meshSfml->transparency = 0.f;

    imagem2.mesh = meshSfml;
    engine.c2d.gui.addObj(imagem2);

    int deltaTime = 0;
    int fade = 0;

    while (!engine.quit())
    {
        if (engine.isStepped())
        {
            engine.pollEvents();

            // Obtém os objetos
            auto& titulo = engine.c2d.gui.get("titulo1");
            auto& imagem1 = engine.c2d.gui.get("imagem1");
            auto& imagem2 = engine.c2d.gui.get("imagem2");

            if (deltaTime <= 100)
            {
                float fadein = (float)fade / 100.f;
                titulo.texture.color = {fadein, fadein, fadein, 1.f};
            }
            else if (deltaTime <= 200)
            {
                float fadeout = (200.f - (float)fade) / 100.f;
                titulo.texture.color = {fadeout, fadeout, fadeout, 1.f};
            }
            else if (deltaTime == 201)
            {
                fade = 1;
                dynamic_cast<Collections2d::Text*>(titulo.mesh)->text = "Metal Himiko";
            }
            else if (deltaTime <= 300)
            {
                float fadein = (float)fade / 100.f;
                titulo.texture.color = {fadein, fadein, fadein, 1.f};
            }
            else if (deltaTime <= 400)
            {
                float fadeout = (200.f - (float)fade) / 100.f;
                titulo.texture.color = {fadeout, fadeout, fadeout, 1.f};
            }
            else if (deltaTime <= 500)
            {
                float fadein = (float)fade / 100.f;
                dynamic_cast<Collections2d::Image*>(imagem1.mesh)->transparency = fadein * 255.f;
                dynamic_cast<Collections2d::Image*>(imagem2.mesh)->transparency = fadein * 255.f;
            }
            else if (deltaTime <= 600)
            {
                float fadeout = (200.f - (float)fade) / 100.f;
                dynamic_cast<Collections2d::Image*>(imagem1.mesh)->transparency = fadeout * 255.f;
                dynamic_cast<Collections2d::Image*>(imagem2.mesh)->transparency = fadeout * 255.f;
            }
            else
            {
                return 0;
            }

            deltaTime += 1;
            fade += 1;

            engine.nextStep();
            engine.c2d.update();
            engine.c3d.update();
        }
        engine.display();
    }
    return 0;
}