#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <random>
#include <iomanip>

using namespace std;

int intro2(Engine &engine)
{
    engine.canvas.gui.clearAll();
    engine.canvas3d.world.scene.clearAll();

    Text titulo;
    titulo.text = "Metal Himiko";
    titulo.name = "titulo1";
    titulo.x = 0.5f;
    titulo.y = 0.5f;
    titulo.align = 1;
    titulo.fontSize = 132;
    titulo.pathFont = "fonts/OstrichSans-Medium.otf";
    titulo.setColor(0.5f, 0.5f, 0.5f, 0.0f);
    engine.canvas.gui.add(titulo);

    random_device rd;
    std::mt19937 gerador(rd());
    int min = 0;
    int max = 3;
    uniform_int_distribution<int> dist(min, max);

    engine.canvas3d.world.camera.x = 0;
    engine.canvas3d.world.camera.y = 7;
    engine.canvas3d.world.camera.z = 0;

    engine.canvas3d.world.camera.rotationX = 270;
    engine.canvas3d.world.camera.rotationY = 0;
    engine.canvas3d.world.camera.rotationZ = 0;

    int count = 0;
    for (int i1 = -7; i1 < 7; i1++)
    {
        for (int i2 = -7; i2 < 7; i2++)
        {
            Object3D cubo;
            cubo.name = "cube" + to_string(count);
            cubo.sca_y = 3;
            cubo.pos_x = i2;
            cubo.pos_z = i1;
            cubo.pos_y = dist(gerador);
            // cubo.pos_y = 0;
            engine.canvas3d.world.scene.add(cubo);
            count++;
        }
    }

    Light luz;
    luz.setPosition(0.f, 10.f, 0.f);
    luz.setAmbient(1.f, 1.f, 1.f, 1.f);
    luz.type = 0;
    luz.name = "luz1";
    engine.canvas3d.world.scene.add(luz);

    int deltaTime = 0;
    int phase = 0;
    float black = 1.f;

    while (!engine.quit())
    {
        if (engine.isStepped())
        {
            engine.pollEvents();

            if (deltaTime <= 200)
            {
                // Cubos descendo
                for (int i = 0; i < 196; i++)
                {
                    Object3D &cubeT = engine.canvas3d.world.scene.getObject3D("cube" + to_string(i));
                    cubeT.pos_y = cubeT.pos_y * 0.8f;
                    if (cubeT.pos_y < 0.1f)
                        cubeT.pos_y = 0.f;
                }
                deltaTime += 2;
            }
            else if (deltaTime == 201)
            {
                // Título aparece
                engine.canvas.gui.getText("titulo1").a = 1.f;
            }
            else if (deltaTime <= 300) // Mudou de 400 para 300
            {
                float progresso = (deltaTime - 201) / 99.0f; // 300-201 = 99 frames
                float intensidade = 1.0f - progresso;

                Light &luz = engine.canvas3d.world.scene.getLight("luz1");
                luz.setAmbient(intensidade, intensidade, intensidade, 1.0f);
                luz.setDiffuse(intensidade, intensidade, intensidade, 1.0f);

                // DEBUG: Mostre o valor no console
                //cout << "Intensidade da luz: " << intensidade << endl;

                // DEBUG: Mude a cor de um cubo para ver se está mudando
                Object3D &cube0 = engine.canvas3d.world.scene.getObject3D("cube0");
                cube0.texture.r = intensidade;
                cube0.texture.g = 0;
                cube0.texture.b = 0;
            }else if (deltaTime == 400){
                return 0;
            }


            deltaTime++;
            engine.nextStep();
            engine.canvas.update();
            engine.canvas3d.update();
        }
        engine.display();
    }
    return 0;
}