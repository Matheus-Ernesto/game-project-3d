// 7 hours spent
// OpenGL choosed
#include <stdio.h>
#include <iostream>
#include <functional>
#include <chrono>
#include <GL/glut.h>
#include <fstream>

using namespace std;

struct Configs{
   int widthWindow = 800;
   int heightWindow = 600;
   string typeWindow = "Windowed";
   bool showFPS = true;

   bool operator==(const Configs& other) const {
      return widthWindow == other.widthWindow &&
             heightWindow == other.heightWindow &&
             typeWindow == other.typeWindow &&
             showFPS == other.showFPS;
   }
};

struct GameConfigs{
   int fixedTimeStepMs = 250;
   string maxDeltaTime = "";
   string useMultiThreading = "";
   string enableDebug = "";
   string muteWhenUnfocused = "";
   string autoSave = "";
};

struct Bindings{
};

class Loader{
public:
   int saveConfig(Configs config){
      ofstream file("config.txt");
      if(!file) return 1;

      file << config.widthWindow << "\n";
      file << config.heightWindow << "\n";
      file << config.typeWindow << "\n";
      file << config.showFPS << "\n";

      file.close();
      return 0;
   }

   Configs loadConfig(){
      Configs config;
      ifstream file("config.txt");
      if(!file) return config;

      file >> config.widthWindow;
      file >> config.heightWindow;
      file >> config.typeWindow;
      file >> config.showFPS;

      file.close();
      return config;
   }
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

   function<void()> gameVoid; //FOR USER DEFINE
   function<void(int, int, int, int)> mouseVoid; //FOR USER DEFINE
   function<void(unsigned char, int, int)> keyboardVoid; //FOR USER DEFINE
   function<void(int, int, int)> specialVoid; //FOR USER DEFINE


   Engine() {
      instance = this;
   }

   static void game(int arg) { 
     if (instance && instance->gameVoid)
         instance->gameVoid();
     glutTimerFunc(timeGameStep,game,0);
   }

   static void keyboard(unsigned char key, int x, int y) { 
     if (instance && instance->keyboardVoid)
         instance->keyboardVoid(key, x, y);
   }

   static void mouse(int button, int state, int x, int y) { 
     if (instance && instance->mouseVoid)
         instance->mouseVoid(button,state,x,y);
   }

   static void special(int key, int x, int y) { 
     if (instance && instance->specialVoid)
         instance->specialVoid(key,x,y);
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

      //RECALL
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glutSwapBuffers();
      glutPostRedisplay();
   }

   void start(int argc, char* argv[]) {
      //DEFINE WINDOWS CONFIGS
      showFps = configs.showFPS;
      timeGameStep = gameConfigs.fixedTimeStepMs;

      glutInit(&argc, argv);
      glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

      glutInitWindowSize(configs.widthWindow, configs.heightWindow);

      glutCreateWindow("Test");

      if(configs.typeWindow == "Fullscreen") {glutFullScreen();}
      glutDisplayFunc(display); // FPS RENDER LOOP
      glutTimerFunc(gameConfigs.fixedTimeStepMs,game,0); //GAME STEP LOOP
      glutKeyboardFunc(keyboard);
      glutMouseFunc(mouse);
      glutSpecialFunc(special);
      glutMainLoop(); 
   }
   
   void reload(){
      if(configs.typeWindow == "Fullscreen") {
         glutFullScreen();
      } else{
         glutReshapeWindow(configs.widthWindow, configs.heightWindow);
      }
   }

};

int gameMenu = 0;
Engine engine;
Configs n_configs;
GameConfigs n_gameConfigs;

void mainGame(){
   Loader loader;
   Configs configs = loader.loadConfig();
   if(configs == n_configs){
      loader.saveConfig(configs); //SAVE BECAUSE DOESNT EXIST THE FILE
   } else {
      engine.configs = configs; //IS DIFFERENT FROM NORMAL, RELOAD
      engine.reload();
   }

   //TO DO
   engine.keyboardVoid = [](unsigned char key, int x, int y){
      cout << "key: " << key << " Mouse Position: (" << x << "," << y << ")." << endl;
   };
   
   
};



int main(int argc, char* argv[])
{
    cout<<"Configs: normal\n";

    cout<<"Engine instances\n";
    Engine engine;
    engine.configs = n_configs;
    engine.gameConfigs = n_gameConfigs;

    /*
    engine.keyboardVoid = [](unsigned char key, int x, int y){
        cout << "key: " << key << " Mouse Position: (" << x << "," << y << ")." << endl;
    };
    engine.mouseVoid = [](int button, int state, int x, int y){
        cout << "button: " << button << " State: " << state << " Mouse Position: (" << x << "," << y << ")." << endl;
    };
    engine.specialVoid = [](int key, int x, int y){
        cout << "key: " << key << " Mouse Position: (" << x << "," << y << ")." << endl;
    };
    */

    engine.gameVoid = [](){
        switch(gameMenu){
            case 0:
               mainGame();
                break;
            default:
                break;
        }
    };

    cout << "Engine start\n";
    engine.start(argc, argv);

    return 0;
}
