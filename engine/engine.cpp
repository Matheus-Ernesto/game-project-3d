// 1 hours spent
// OpenGL choosed
#include <stdio.h>
#include <iostream>
//#include <GL/glut.h>

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
    
    void start() {
    }
};

int main(int argc, char **argv)
{
    std::cout<<"Engine launch\n";
    /*
    glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB);
  glutInitWindowSize(210, 210);
  int main_w = glutCreateWindow("4 subwindows");
  glutDisplayFunc(display);
  glutVisibilityFunc(vis);
  glutReshapeFunc(reshape);
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glutMainLoop();
  */
    std::cout<<"Window launch\n";
    return 0;
}