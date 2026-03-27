#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <random>
#include <iomanip>

using namespace std;

int intro2(Engine &engine)
{
    engine.c2d.gui.deleteAll();
    engine.c3d.world.scene.clearAll();

    engine.audio.setup();
    engine.video.setup();

    engine.audio.addSong("tema_longo", "assets/tema_longo.ogg", 80);
    engine.audio.playSong("tema_longo", false);

    engine.video.createFramePack("minha_intro",
                                 "assets/frames", // pasta dos frames
                                 "frame",         // nome base
                                 "png");          // formato

    // Toca o vídeo a 24 fps
    engine.video.playFramePack("minha_intro", 1, "intro");

    int deltaTime = 0;

    while (!engine.quit())
    {
        if (engine.isStepped())
        {
            engine.pollEvents();

            if (!engine.video.isPlaying("intro"))
            {

                engine.video.playFramePack("minha_intro", 1, "intro");
            }

            deltaTime++;
            engine.nextStep();
            engine.c2d.update();
            engine.c3d.update();
        }
        engine.display();
    }
    return 0;
}