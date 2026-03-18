#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <string>

using namespace std;

int configuracoes(Engine &engine)
{
    engine.canvas.gui.clearAll();
    engine.canvas3d.world.scene.clearAll();

    Text titulo;
    titulo.text = "Configurações";
    titulo.name = "titulo";
    titulo.x = 0.0f;
    titulo.y = 0.2f;
    titulo.fontSize = 36;
    titulo.align = 0;
    titulo.setColor(0.0f, 0.0f, 0.0f);
    engine.canvas.gui.add(titulo);

    Text video;
    video.text = "Vídeo";
    video.name = "video";
    video.x = 0.0f;
    video.y = 0.6f;
    video.align = 0;
    video.setColor(0.0f, 0.0f, 0.0f);
    engine.canvas.gui.add(video);

    Text outros;
    outros.text = "Outros";
    outros.name = "outros";
    outros.x = 0.0f;
    outros.y = 0.7f;
    outros.align = 0;
    outros.setColor(0.0f, 0.0f, 0.0f);
    engine.canvas.gui.add(outros);

    Text saves;
    saves.text = "Saves";
    saves.name = "saves";
    saves.x = 0.0f;
    saves.y = 0.8f;
    saves.align = 0;
    saves.setColor(0.0f, 0.0f, 0.0f);
    engine.canvas.gui.add(saves);

    Text sair;
    sair.text = "Sair";
    sair.name = "sair";
    sair.x = 0.0f;
    sair.y = 0.9f;
    sair.align = 0;
    sair.setColor(0.0f, 0.0f, 0.0f);
    engine.canvas.gui.add(sair);

    Object3D cube;
    cube.name = "cube";
    cube.pos_x = 3.f;
    cube.pos_z = 3.f;

    engine.canvas3d.world.scene.add(cube);

    int deltaTime = 0;

    int option = 0;
    vector<string> texts = {"titulo", "video", "outros", "saves", "sair"};
    vector<string> options = {"video", "outros", "saves", "sair"};

    while (!engine.quit())
    {
        if (engine.isStepped())
        {
            engine.pollEvents();
            if (deltaTime < 100)
            {
                fadeLeftOnePitch(texts, engine);
            }
            else
            {
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
                    return option;
                }
                if (option < 0)
                    option = 3;
                if (option > 3)
                    option = 0;

                changeSelectedOption(options, option, engine);
            }

            deltaTime += 1;

            engine.canvas3d.world.scene.getObject3D("cube").rot_x += 1.f;
            engine.canvas3d.world.scene.getObject3D("cube").rot_y += 1.f;
            engine.canvas3d.world.scene.getObject3D("cube").rot_z += 1.f;

            engine.nextStep();
            engine.canvas.update();
            engine.canvas3d.update();
        }
        engine.display();
    }
    return 0;
};

int video(Engine &engine, Configs &configs)
{
    engine.canvas.gui.clearAll();
    engine.canvas3d.world.scene.clearAll();

    Text titulo;
    titulo.text = "Vídeo";
    titulo.name = "titulo";
    titulo.x = 0.0f;
    titulo.y = 0.2f;
    titulo.fontSize = 36;
    titulo.align = 0;
    titulo.setColor(0.0f, 0.0f, 0.0f);
    engine.canvas.gui.add(titulo);

    Text resolucao;
    resolucao.text = "Resolução";
    resolucao.name = "resolucao";
    resolucao.x = 0.0f;
    resolucao.y = 0.6f;
    resolucao.align = 0;
    resolucao.setColor(0.0f, 0.0f, 0.0f);
    engine.canvas.gui.add(resolucao);

    Text tela_cheia;
    tela_cheia.text = "Tela Cheia";
    tela_cheia.name = "tela_cheia";
    tela_cheia.x = 0.0f;
    tela_cheia.y = 0.7f;
    tela_cheia.align = 0;
    tela_cheia.setColor(0.0f, 0.0f, 0.0f);
    engine.canvas.gui.add(tela_cheia);

    Text mostrar_fps;
    mostrar_fps.text = "Mostrar FPS";
    mostrar_fps.name = "mostrar_fps";
    mostrar_fps.x = 0.0f;
    mostrar_fps.y = 0.8f;
    mostrar_fps.align = 0;
    mostrar_fps.setColor(0.0f, 0.0f, 0.0f);
    engine.canvas.gui.add(mostrar_fps);

    Text modo_resolucao;
    modo_resolucao.text = "";
    modo_resolucao.name = "modo_resolucao";
    modo_resolucao.x = 0.4f;
    modo_resolucao.y = 0.6f;
    modo_resolucao.align = 0;
    modo_resolucao.setColor(0.0f, 0.0f, 0.0f);
    engine.canvas.gui.add(modo_resolucao);

    Text modo_tela;
    modo_tela.text = "";
    modo_tela.name = "modo_tela";
    modo_tela.x = 0.4f;
    modo_tela.y = 0.7f;
    modo_tela.align = 0;
    modo_tela.setColor(0.0f, 0.0f, 0.0f);
    engine.canvas.gui.add(modo_tela);

    Text modo_fps;
    modo_fps.text = "";
    modo_fps.name = "modo_fps";
    modo_fps.x = 0.4f;
    modo_fps.y = 0.8f;
    modo_fps.align = 0;
    modo_fps.setColor(0.0f, 0.0f, 0.0f);
    engine.canvas.gui.add(modo_fps);

    Text sair;
    sair.text = "Sair/Aplicar";
    sair.name = "sair";
    sair.x = 0.0f;
    sair.y = 0.9f;
    sair.align = 0;
    sair.setColor(0.0f, 0.0f, 0.0f);
    engine.canvas.gui.add(sair);

    Object3D cube;
    cube.name = "cube";
    cube.pos_x = 3.f;
    cube.pos_z = 3.f;

    engine.canvas3d.world.scene.add(cube);

    int deltaTime = 0;

    int option = 0;
    vector<string> texts = {"titulo", "resolucao", "tela_cheia", "mostrar_fps", "modo_resolucao", "modo_tela", "modo_fps", "sair"};
    vector<string> options = {"resolucao", "tela_cheia", "mostrar_fps", "sair"};

    while (!engine.quit())
    {
        if (engine.isStepped())
        {
            engine.pollEvents();
            if (deltaTime < 100)
            {
                fadeLeftOnePitch(texts, engine);
            }
            else
            {
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
                    switch (option)
                    {
                    case 0:
                        // Resolução
                        switch (configs.widthWindow)
                        {
                        case 640:
                            configs.widthWindow = 800;
                            configs.heightWindow = 600;
                            break;
                        case 800:
                            configs.widthWindow = 1024;
                            configs.heightWindow = 576;
                            break;
                        case 1024:
                            configs.widthWindow = 1280;
                            configs.heightWindow = 720;
                            break;
                        case 1280:
                            configs.widthWindow = 1366;
                            configs.heightWindow = 768;
                            break;
                        case 1366:
                            configs.widthWindow = 1600;
                            configs.heightWindow = 900;
                            break;
                        case 1600:
                            configs.widthWindow = 1920;
                            configs.heightWindow = 1080;
                            break;
                        case 1920:
                            configs.widthWindow = 2560;
                            configs.heightWindow = 1440;
                            break;
                        case 2560:
                            configs.widthWindow = 640;
                            configs.heightWindow = 480;
                            break;
                        default:
                            break;
                        }
                        break;
                    case 1:
                        configs.fullscreen = configs.fullscreen ? false : true;
                        break;
                    case 2:
                        configs.showFPS = configs.showFPS ? false : true;
                        break;
                    case 3:
                        return 0;
                        break;
                    default:
                        break;
                    }
                }

                if (option < 0)
                    option = 3;
                if (option > 3)
                    option = 0;

                changeSelectedOption(options, option, engine);
                engine.canvas.gui.getText("modo_resolucao").text = "";
                engine.canvas.gui.getText("modo_tela").text = "";
                engine.canvas.gui.getText("modo_fps").text = "";

                if (option == 0)
                {
                    engine.canvas.gui.getText("modo_resolucao").text = to_string(configs.widthWindow) + "x" + to_string(configs.heightWindow);
                    engine.canvas.gui.getText("modo_resolucao").setColor(0.3f, 0.3f, 1.f);
                }
                else if (option == 1)
                {
                    engine.canvas.gui.getText("modo_tela").text = (configs.fullscreen == 0) ? "Modo Janela" : "Modo Tela Cheia Exclus.";
                    engine.canvas.gui.getText("modo_tela").setColor(0.3f, 0.3f, 1.f);
                }
                else if (option == 2)
                {
                    engine.canvas.gui.getText("modo_fps").text = (configs.showFPS == 1) ? "Exibir" : "Ocultar";
                    engine.canvas.gui.getText("modo_fps").setColor(0.3f, 0.3f, 1.f);
                }
            }

            deltaTime += 1;

            engine.canvas3d.world.scene.getObject3D("cube").rot_x += 1.f;
            engine.canvas3d.world.scene.getObject3D("cube").rot_y += 1.f;
            engine.canvas3d.world.scene.getObject3D("cube").rot_z += 1.f;

            engine.nextStep();
            engine.canvas.update();
            engine.canvas3d.update();
        }
        engine.display();
    }
    return 0;
};

int outros(Engine &engine, Configs &configs)
{
    engine.canvas.gui.clearAll();
    engine.canvas3d.world.scene.clearAll();

    Text titulo;
    titulo.text = "Outros";
    titulo.name = "titulo";
    titulo.x = 0.0f;
    titulo.y = 0.2f;
    titulo.fontSize = 36;
    titulo.align = 0;
    titulo.setColor(0.0f, 0.0f, 0.0f);
    engine.canvas.gui.add(titulo);

    Text audio;
    audio.text = "Volume geral:";
    audio.name = "audio";
    audio.x = 0.0f;
    audio.y = 0.8f;
    audio.align = 0;
    audio.setColor(0.0f, 0.0f, 0.0f);
    engine.canvas.gui.add(audio);

    Text volume;
    volume.text = "";
    volume.name = "volume";
    volume.x = 0.6f;
    volume.y = 0.8f;
    volume.align = 0;
    volume.setColor(0.0f, 0.0f, 0.0f);
    engine.canvas.gui.add(volume);

    Text sair;
    sair.text = "Sair";
    sair.name = "sair";
    sair.x = 0.0f;
    sair.y = 0.9f;
    sair.align = 0;
    sair.setColor(0.0f, 0.0f, 0.0f);
    engine.canvas.gui.add(sair);

    int deltaTime = 0;

    int option = 0;
    vector<string> texts = {"titulo", "audio", "volume", "sair"};
    vector<string> options = {"audio", "sair"};

    while (!engine.quit())
    {
        if (engine.isStepped())
        {
            engine.pollEvents();

            if (deltaTime < 100)
            {
                fadeLeftOnePitch(texts, engine);
            }
            else
            {
                if (engine.getKeyDown(sf::Keyboard::W) || engine.getKeyDown(sf::Keyboard::Up))
                {
                    option--;
                }
                if (engine.getKeyDown(sf::Keyboard::S) || engine.getKeyDown(sf::Keyboard::Down))
                {
                    option++;
                }
                if (engine.getKeyDown(sf::Keyboard::A) || engine.getKeyDown(sf::Keyboard::Left))
                {
                    if (configs.audioVolume > 0 && option == 0)
                    {
                        configs.audioVolume--;
                    }
                }
                if (engine.getKeyDown(sf::Keyboard::D) || engine.getKeyDown(sf::Keyboard::Right))
                {
                    if (configs.audioVolume < 10 && option == 0)
                    {
                        configs.audioVolume++;
                    }
                }
                if (engine.getKeyDown(sf::Keyboard::Enter))
                {
                    return option;
                }
                if (option < 0)
                    option = 1;
                if (option > 1)
                    option = 0;

                for (auto obj : texts)
                {
                    engine.canvas.gui.getText(obj).setColor(1.f, 1.f, 1.f);
                }

                if (option == 0)
                {
                    engine.canvas.gui.getText("audio").setColor(0.3f, 0.3f, 1.f);
                    engine.canvas.gui.getText("volume").setColor(0.3f, 0.3f, 1.f);

                    engine.canvas.gui.getText("volume").text = to_string(configs.audioVolume);
                }
                if (option == 1)
                {
                    engine.canvas.gui.getText("volume").text = "";
                    engine.canvas.gui.getText("sair").setColor(0.3f, 0.3f, 1.f);
                }
            }

            deltaTime += 1;

            engine.nextStep();
            engine.canvas.update();
            engine.canvas3d.update();
        }
        engine.display();
    }
    return 0;
};

int saves(Engine &engine)
{
    engine.canvas.gui.clearAll();
    engine.canvas3d.world.scene.clearAll();

    Text titulo;
    titulo.text = "Saves";
    titulo.name = "titulo";
    titulo.x = 0.0f;
    titulo.y = 0.2f;
    titulo.fontSize = 36;
    titulo.align = 0;
    titulo.setColor(0.0f, 0.0f, 0.0f);
    engine.canvas.gui.add(titulo);

    Text aviso;
    aviso.text = "Ainda não implementado...";
    aviso.name = "aviso";
    aviso.x = 0.0f;
    aviso.y = 0.8f;
    aviso.align = 0;
    aviso.setColor(0.0f, 0.0f, 0.0f);
    engine.canvas.gui.add(aviso);

    Text sair;
    sair.text = "Sair";
    sair.name = "sair";
    sair.x = 0.0f;
    sair.y = 0.9f;
    sair.align = 0;
    sair.setColor(0.0f, 0.0f, 0.0f);
    engine.canvas.gui.add(sair);

    Object3D cube;
    cube.name = "cube";
    cube.pos_x = 3.f;
    cube.pos_z = 3.f;

    engine.canvas3d.world.scene.add(cube);

    int deltaTime = 0;

    vector<string> texts = {"titulo", "aviso", "sair"};
    vector<string> options = {"sair"};

    while (!engine.quit())
    {
        if (engine.isStepped())
        {
            engine.pollEvents();
            if (deltaTime < 100)
            {
                fadeLeftOnePitch(texts, engine);
            }
            else
            {
                engine.canvas.gui.getText("sair").setColor(0.3f, 0.3f, 1.f);
                if (engine.getKeyDown(sf::Keyboard::Enter))
                {
                    return 0;
                }
            }

            deltaTime += 1;

            engine.canvas3d.world.scene.getObject3D("cube").rot_x += 1.f;
            engine.canvas3d.world.scene.getObject3D("cube").rot_y += 1.f;
            engine.canvas3d.world.scene.getObject3D("cube").rot_z += 1.f;

            engine.nextStep();
            engine.canvas.update();
            engine.canvas3d.update();
        }
        engine.display();
    }
    return 0;
};