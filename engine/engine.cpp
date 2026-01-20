// 3 hours spent
// OpenGL choosed
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <GL/glut.h>

using namespace std;

struct Configs{
    int widthWindow = 800;
    int heightWindow = 600;
    string typeWindow = "Windowed";
    int fpsLimit = 60;
    bool vsync = false;
    int monitorIndex = 0;
    string aspectRatio = "normal";
    string quality = "normal";
};

struct Bindings{
};

struct GameConfigs{
    string fixedTimeStep = "";
    string maxDeltaTime = "";
    string useMultiThreading = "";
    string enableDebug = "";
    string muteWhenUnfocused = "";
    string autoSave = "";
};

class SaveLoader{
    
};

class ConfigLoader{
    
};

class Engine{
public:
    static int totalFrames;
    static std::chrono::high_resolution_clock::time_point last;

    static void display() {
        totalFrames++;
        int fps = 0;

        auto now = std::chrono::high_resolution_clock::now();

        auto diff = std::chrono::duration<double>(now - last).count();

        if(diff >= 1.0){
            fps = totalFrames / 60;
            totalFrames = 0;
            last = now;
            string fpsTotal = "FPS" + to_string(fps);
            glutSetWindowTitle(fpsTotal.c_str());
        }
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	    glutSwapBuffers();
        glutPostRedisplay();
    }

    void start(int argc, char* argv[]) {
	    glutInit(&argc, argv);
	    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	    glutCreateWindow("Test");
	    glutDisplayFunc(display);
	    glutMainLoop(); 
    }
};

int Engine::totalFrames = 0;
std::chrono::high_resolution_clock::time_point Engine::last = std::chrono::high_resolution_clock::now();

int main(int argc, char* argv[])
{
    std::cout<<"Engine launch\n";
    std::cout<<"Window launch\n";
    Engine engine;
    engine.start(argc, argv);
    return 0;
}
