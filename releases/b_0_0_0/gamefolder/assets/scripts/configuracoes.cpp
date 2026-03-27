#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <string>

using namespace std;

// Função auxiliar para criar um objeto de texto na GUI
void addText(Engine& engine, const string& name, const string& text, float x, float y, int fontSize, int align, float r, float g, float b, float a = 1.0f) {
    Collections2d::Object obj;
    obj.name = name;
    obj.mesh = new Collections2d::Text();
    auto* textMesh = dynamic_cast<Collections2d::Text*>(obj.mesh);
    textMesh->text = text;
    textMesh->fontSize = fontSize;
    textMesh->pathFont = "fonts/arial.ttf"; // ajuste conforme necessário
    obj.transform.position = {x, y, 0.f};
    obj.transform.align = align;
    obj.texture.color = {r, g, b, a};
    engine.c2d.gui.add(obj);
}

int configuracoes(Engine &engine)
{
    engine.c2d.gui.deleteAll();
    engine.c3d.world.scene.clearAll();

    addText(engine, "titulo", "Configurações", 0.0f, 0.2f, 36, 0, 0.0f, 0.0f, 0.0f);
    addText(engine, "video", "Vídeo", 0.0f, 0.6f, 24, 0, 0.0f, 0.0f, 0.0f);
    addText(engine, "outros", "Outros", 0.0f, 0.7f, 24, 0, 0.0f, 0.0f, 0.0f);
    addText(engine, "saves", "Saves", 0.0f, 0.8f, 24, 0, 0.0f, 0.0f, 0.0f);
    addText(engine, "sair", "Sair", 0.0f, 0.9f, 24, 0, 0.0f, 0.0f, 0.0f);

    Collections3d::Object cube;
    cube.name = "cube";
    cube.transform.position = {3.f, 0.f, 3.f};
    cube.mesh = make_unique<Collections3d::Cube>(); // assumindo que Cube é uma subclasse de Mesh
    engine.c3d.world.scene.add(cube);

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
                fadeLeftOnePitch(texts, engine); // corrigido em utils.cpp
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
                if (option < 0) option = 3;
                if (option > 3) option = 0;

                changeSelectedOption(options, option, engine); // corrigido em utils.cpp
            }

            deltaTime += 1;

            auto& cubeObj = engine.c3d.world.scene.getObject("cube");
            cubeObj.transform.rotation.x += 1.f;
            cubeObj.transform.rotation.y += 1.f;
            cubeObj.transform.rotation.z += 1.f;

            engine.nextStep();
            engine.c2d.update();
            engine.c3d.update();
        }
        engine.display();
    }
    return 0;
}

int video(Engine &engine, Configs &configs)
{
    engine.c2d.gui.deleteAll();
    engine.c3d.world.scene.clearAll();

    addText(engine, "titulo", "Vídeo", 0.0f, 0.2f, 36, 0, 0.0f, 0.0f, 0.0f);
    addText(engine, "resolucao", "Resolução", 0.0f, 0.6f, 24, 0, 0.0f, 0.0f, 0.0f);
    addText(engine, "tela_cheia", "Tela Cheia", 0.0f, 0.7f, 24, 0, 0.0f, 0.0f, 0.0f);
    addText(engine, "mostrar_fps", "Mostrar FPS", 0.0f, 0.8f, 24, 0, 0.0f, 0.0f, 0.0f);
    addText(engine, "modo_resolucao", "", 0.4f, 0.6f, 20, 0, 0.0f, 0.0f, 0.0f);
    addText(engine, "modo_tela", "", 0.4f, 0.7f, 20, 0, 0.0f, 0.0f, 0.0f);
    addText(engine, "modo_fps", "", 0.4f, 0.8f, 20, 0, 0.0f, 0.0f, 0.0f);
    addText(engine, "sair", "Sair/Aplicar", 0.0f, 0.9f, 24, 0, 0.0f, 0.0f, 0.0f);

    Collections3d::Object cube;
    cube.name = "cube";
    cube.transform.position = {3.f, 0.f, 3.f};
    cube.mesh = make_unique<Collections3d::Cube>();
    engine.c3d.world.scene.add(cube);

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
                    case 0: // Resolução
                        switch (configs.widthWindow)
                        {
                        case 640: configs.widthWindow = 800; configs.heightWindow = 600; break;
                        case 800: configs.widthWindow = 1024; configs.heightWindow = 576; break;
                        case 1024: configs.widthWindow = 1280; configs.heightWindow = 720; break;
                        case 1280: configs.widthWindow = 1366; configs.heightWindow = 768; break;
                        case 1366: configs.widthWindow = 1600; configs.heightWindow = 900; break;
                        case 1600: configs.widthWindow = 1920; configs.heightWindow = 1080; break;
                        case 1920: configs.widthWindow = 2560; configs.heightWindow = 1440; break;
                        case 2560: configs.widthWindow = 640; configs.heightWindow = 480; break;
                        default: break;
                        }
                        break;
                    case 1: // Tela cheia
                        configs.fullscreen = !configs.fullscreen;
                        break;
                    case 2: // Mostrar FPS
                        configs.showFPS = !configs.showFPS;
                        break;
                    case 3: // Sair
                        return 0;
                    default: break;
                    }
                }

                if (option < 0) option = 3;
                if (option > 3) option = 0;

                changeSelectedOption(options, option, engine);

                // Limpa os textos de status
                auto& modoResolucao = engine.c2d.gui.get("modo_resolucao");
                auto& modoTela = engine.c2d.gui.get("modo_tela");
                auto& modoFps = engine.c2d.gui.get("modo_fps");

                if (auto* textMesh = dynamic_cast<Collections2d::Text*>(modoResolucao.mesh))
                    textMesh->text = "";
                if (auto* textMesh = dynamic_cast<Collections2d::Text*>(modoTela.mesh))
                    textMesh->text = "";
                if (auto* textMesh = dynamic_cast<Collections2d::Text*>(modoFps.mesh))
                    textMesh->text = "";

                if (option == 0)
                {
                    if (auto* textMesh = dynamic_cast<Collections2d::Text*>(modoResolucao.mesh))
                        textMesh->text = to_string(configs.widthWindow) + "x" + to_string(configs.heightWindow);
                    modoResolucao.texture.color = {0.3f, 0.3f, 1.f, 1.f};
                }
                else if (option == 1)
                {
                    if (auto* textMesh = dynamic_cast<Collections2d::Text*>(modoTela.mesh))
                        textMesh->text = (configs.fullscreen == 0) ? "Modo Janela" : "Modo Tela Cheia Exclus.";
                    modoTela.texture.color = {0.3f, 0.3f, 1.f, 1.f};
                }
                else if (option == 2)
                {
                    if (auto* textMesh = dynamic_cast<Collections2d::Text*>(modoFps.mesh))
                        textMesh->text = (configs.showFPS == 1) ? "Exibir" : "Ocultar";
                    modoFps.texture.color = {0.3f, 0.3f, 1.f, 1.f};
                }
            }

            deltaTime += 1;

            auto& cubeObj = engine.c3d.world.scene.getObject("cube");
            cubeObj.transform.rotation.x += 1.f;
            cubeObj.transform.rotation.y += 1.f;
            cubeObj.transform.rotation.z += 1.f;

            engine.nextStep();
            engine.c2d.update();
            engine.c3d.update();
        }
        engine.display();
    }
    return 0;
}

int outros(Engine &engine, Configs &configs)
{
    engine.c2d.gui.deleteAll();
    engine.c3d.world.scene.clearAll();

    addText(engine, "titulo", "Outros", 0.0f, 0.2f, 36, 0, 0.0f, 0.0f, 0.0f);
    addText(engine, "audio", "Volume geral:", 0.0f, 0.8f, 24, 0, 0.0f, 0.0f, 0.0f);
    addText(engine, "volume", "", 0.6f, 0.8f, 24, 0, 0.0f, 0.0f, 0.0f);
    addText(engine, "sair", "Sair", 0.0f, 0.9f, 24, 0, 0.0f, 0.0f, 0.0f);

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
                        auto& volumeObj = engine.c2d.gui.get("volume");
                        if (auto* textMesh = dynamic_cast<Collections2d::Text*>(volumeObj.mesh))
                            textMesh->text = to_string(configs.audioVolume);
                    }
                }
                if (engine.getKeyDown(sf::Keyboard::D) || engine.getKeyDown(sf::Keyboard::Right))
                {
                    if (configs.audioVolume < 10 && option == 0)
                    {
                        configs.audioVolume++;
                        auto& volumeObj = engine.c2d.gui.get("volume");
                        if (auto* textMesh = dynamic_cast<Collections2d::Text*>(volumeObj.mesh))
                            textMesh->text = to_string(configs.audioVolume);
                    }
                }
                if (engine.getKeyDown(sf::Keyboard::Enter))
                {
                    return option;
                }
                if (option < 0) option = 1;
                if (option > 1) option = 0;

                for (auto objName : texts)
                {
                    auto& obj = engine.c2d.gui.get(objName);
                    obj.texture.color = {1.f, 1.f, 1.f, 1.f};
                }

                if (option == 0)
                {
                    auto& audioObj = engine.c2d.gui.get("audio");
                    auto& volumeObj = engine.c2d.gui.get("volume");
                    audioObj.texture.color = {0.3f, 0.3f, 1.f, 1.f};
                    volumeObj.texture.color = {0.3f, 0.3f, 1.f, 1.f};
                    if (auto* textMesh = dynamic_cast<Collections2d::Text*>(volumeObj.mesh))
                        textMesh->text = to_string(configs.audioVolume);
                }
                if (option == 1)
                {
                    auto& volumeObj = engine.c2d.gui.get("volume");
                    if (auto* textMesh = dynamic_cast<Collections2d::Text*>(volumeObj.mesh))
                        textMesh->text = "";
                    auto& sairObj = engine.c2d.gui.get("sair");
                    sairObj.texture.color = {0.3f, 0.3f, 1.f, 1.f};
                }
            }

            deltaTime += 1;

            engine.nextStep();
            engine.c2d.update();
            engine.c3d.update();
        }
        engine.display();
    }
    return 0;
}

int saves(Engine &engine)
{
    engine.c2d.gui.deleteAll();
    engine.c3d.world.scene.clearAll();

    addText(engine, "titulo", "Saves", 0.0f, 0.2f, 36, 0, 0.0f, 0.0f, 0.0f);
    addText(engine, "aviso", "Ainda não implementado...", 0.0f, 0.8f, 24, 0, 0.0f, 0.0f, 0.0f);
    addText(engine, "sair", "Sair", 0.0f, 0.9f, 24, 0, 0.0f, 0.0f, 0.0f);

    Collections3d::Object cube;
    cube.name = "cube";
    cube.transform.position = {3.f, 0.f, 3.f};
    cube.mesh = make_unique<Collections3d::Cube>();
    engine.c3d.world.scene.add(cube);

    int deltaTime = 0;
    vector<string> texts = {"titulo", "aviso", "sair"};

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
                auto& sairObj = engine.c2d.gui.get("sair");
                sairObj.texture.color = {0.3f, 0.3f, 1.f, 1.f};
                if (engine.getKeyDown(sf::Keyboard::Enter))
                {
                    return 0;
                }
            }

            deltaTime += 1;

            auto& cubeObj = engine.c3d.world.scene.getObject("cube");
            cubeObj.transform.rotation.x += 1.f;
            cubeObj.transform.rotation.y += 1.f;
            cubeObj.transform.rotation.z += 1.f;

            engine.nextStep();
            engine.c2d.update();
            engine.c3d.update();
        }
        engine.display();
    }
    return 0;
}