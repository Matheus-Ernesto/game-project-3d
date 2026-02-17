#include <iostream>
#include <fstream>

using namespace std;

// BASIC CONFIGS FOR ENGINE
struct Configs
{
    int widthWindow = 800;
    int heightWindow = 600;
    int audioVolume = 10;
    bool fullscreen = false;
    bool showFPS = true;

    bool operator==(const Configs &other) const
    {
        return widthWindow == other.widthWindow &&
               heightWindow == other.heightWindow &&
               fullscreen == other.fullscreen &&
               showFPS == other.showFPS &&
               audioVolume == other.audioVolume;
    }
};

// CONFIG LOADER FOR CONFIG.TXT
class Loader
{
public:
    int saveConfig(Configs config)
    {
        ofstream file("config.txt");
        if (!file)
            return 1;

        file << config.widthWindow << "\n";
        file << config.heightWindow << "\n";
        file << config.fullscreen << "\n";
        file << config.showFPS << "\n";
        file << config.audioVolume << "\n";

        file.close();
        return 0;
    }

    Configs loadConfig()
    {
        Configs config;
        ifstream file("config.txt");
        if (!file)
            return config;

        file >> config.widthWindow;
        file >> config.heightWindow;
        file >> config.fullscreen;
        file >> config.showFPS;
        file >> config.audioVolume;

        file.close();
        return config;
    }
};
