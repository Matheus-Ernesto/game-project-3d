// 6 hours spent
// OpenGL choosed
#include <stdio.h>
#include <iostream>
#include <functional>
#include <chrono>
#include <GL/glut.h>

using namespace std;

struct Configs{
    int widthWindow = 800;
    int heightWindow = 600;
    string typeWindow = "Windowed";
    bool showFPS = true;
};

struct GameConfigs{
    int fixedTimeStepMs = 500;
    string maxDeltaTime = "";
    string useMultiThreading = "";
    string enableDebug = "";
    string muteWhenUnfocused = "";
    string autoSave = "";
};

struct Bindings{
};

class SaveLoader{
    
};

class ConfigLoader{
    
};

class Engine{
private:
    inline static int totalFrames = 0;
    inline static chrono::high_resolution_clock::time_point last = chrono::high_resolution_clock::now();
    inline static int fps = 0;
    inline static bool showFps = false;
    inline static Engine* instance = nullptr;
    inline static int timeGameStep = 0;

public:
    Configs configs;
    GameConfigs gameConfigs;
    std::function<void()> userIdle; //FOR USER DEFINE

    Engine() {
        instance = this;
    }

    static void internalIdle(int arg) { 
        if (instance && instance->userIdle)
            instance->userIdle();
        glutTimerFunc(timeGameStep,internalIdle,0);
    }

    
    static void display() {
        //FPS
        if(showFps){
            totalFrames++;

            auto now = chrono::high_resolution_clock::now();
            auto diff = chrono::duration<double>(now - last).count();

            if(diff >= 1.0){
                fps = totalFrames;
                totalFrames = 0;
                last = now;
                string fpsTotal = "FPS: " + to_string(fps);
                cout<<fpsTotal<<"\n";
            }
        }
        //TO DO

        //RECALL
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	    glutSwapBuffers();
        glutPostRedisplay();
    }

    void start(int argc, char* argv[]) {
        //TO DO


        //DEFINE WINDOWS CONFIGS
        showFps = configs.showFPS;
        timeGameStep = gameConfigs.fixedTimeStepMs;

	    glutInit(&argc, argv);
	    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

        glutInitWindowSize(configs.widthWindow, configs.heightWindow);

        glutCreateWindow("Test");

        if(configs.typeWindow == "Fullscreen") {glutFullScreen();}
	    glutDisplayFunc(display); // FPS RENDER LOOP
        glutTimerFunc(gameConfigs.fixedTimeStepMs,internalIdle,0); //GAME STEP LOOP
	    glutMainLoop(); 
    }
};

int main(int argc, char* argv[])
{
    cout<<"Configs: normal\n";
    Configs n_configs;
    GameConfigs n_gameConfigs;

    cout<<"Engine instances\n";
    Engine engine;
    engine.configs = n_configs;
    engine.gameConfigs = n_gameConfigs;

    engine.userIdle = [](){
        cout << "OK Game Step" << endl;
    };

    cout<<"Engine start\n";
    engine.start(argc, argv);

    return 0;
}
