// 1.5 hours spent
// OpenGL choosed
#include <stdio.h>
#include <iostream>
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

    void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glutSwapBuffers();
    }

    void start() {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Test");
	glutDisplayFunc(display);
	glutMainLoop(); 
    }
};

int main()
{
    std::cout<<"Engine launch\n";
    std::cout<<"Window launch\n";
    Engine engine;
    engine.start();
    return 0;
}
