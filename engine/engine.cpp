// 13 hours spent
// OpenGL choosed
#include <stdio.h>
#include <iostream>
#include <functional>
#include <chrono>
#include <GL/glut.h>
#include <fstream>
#include <cstring>
#include <vector>

using namespace std;

struct Configs
{
   int widthWindow = 800;
   int heightWindow = 600;
   bool fullscreen = false;
   bool showFPS = true;

   bool operator==(const Configs &other) const
   {
      return widthWindow == other.widthWindow &&
             heightWindow == other.heightWindow &&
             fullscreen == other.fullscreen &&
             showFPS == other.showFPS;
   }
};

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

      file.close();
      return config;
   }
};

class Canvas
{
public:
   inline static int totalFrames = 0;
   inline static chrono::high_resolution_clock::time_point last = chrono::high_resolution_clock::now();
   inline static int fps = 0;

   static constexpr int ALIGN_CENTER = 0;
   struct Color{
      float r, g, b;
   };

   struct Text{
      int x;
      int y;
      string text;
      void *font = GLUT_BITMAP_TIMES_ROMAN_24;
      Color color;
      int position;
   };

   class Drawable{
   public:
      Text* text = nullptr;
      void draw(){
         if(text){
            glColor3f(text->color.r, text->color.g, text->color.b);
            
            int width_text = 0;
            for (int i = 0; text->text[i] != '\0'; i++)
            {
               width_text += glutBitmapWidth(text->font, text->text[i]);
            }
            int w = width_text;

            if (text->position == 0)
            { // CENTER
               text->x -= w / 2;
            }
            else if (text->position == 1)
            { // RIGHT
               text->x -= w;
            }

            glRasterPos2i(text->x, text->y);

            for (int i = 0; text->text[i] != '\0'; i++)
            {
               glutBitmapCharacter(text->font, text->text[i]);
            }
         }
      }
   };

   vector<Drawable> draws;


   void apply(int width, int height, bool showFps)
   {
      glClear(GL_COLOR_BUFFER_BIT);
      // FPS
      if (showFps)
      {
         totalFrames++;

         auto now = chrono::high_resolution_clock::now();
         auto diff = chrono::duration<double>(now - last).count();
         string fpsTotal = "FPS: " + to_string(fps);

         if (diff >= 1.0)
         {
            fps = totalFrames / diff;
            totalFrames = 0;
            last = now;
         }

         glViewport(0, 0, width, height);
         glMatrixMode(GL_PROJECTION);
         glLoadIdentity();
         gluOrtho2D(0, width, height, 0);
         int len, i;
         void *font = GLUT_BITMAP_TIMES_ROMAN_24;

         glRasterPos2f(0, 24);
         len = (int)strlen(fpsTotal.c_str());
         for (i = 0; i < len; i++)
         {
            glutBitmapCharacter(font, fpsTotal[i]);
         }
      }
      //DRAW CHILDS 2D:
      // for(auto& draw : draws){
      //    draw.draw();
      // }
   }
   //glMatrixMode(GL_MODELVIEW);

   void text(int x, int y, const char *text, int align,float r, float g, float b)
   {
      Drawable t_draw;
      t_draw.text->color.r = r;
      t_draw.text->color.g = g;
      t_draw.text->color.b = b;
      t_draw.text->position = align;
      t_draw.text->x = x;
      t_draw.text->y = y;
      t_draw.text->text = text;

      draws.push_back(t_draw);

   }
};

class Engine
{
private:
   inline static Engine *instance = nullptr;

public:
   inline static int timeGameStep = 16;
   inline static bool fullscreen = false;
   inline static int width = 100;
   inline static int height = 100;
   inline static bool showFPS = false;
   inline static string windowName = "Test";
   inline static Canvas canvas;

   function<void()> gameVoid;                            // FOR USER DEFINE
   function<void(int, int, int, int)> mouseVoid;         // FOR USER DEFINE
   function<void(int, int)> motionVoid;                  // FOR USER DEFINE
   function<void(int, int)> passiveMotionVoid;           // FOR USER DEFINE
   function<void(unsigned char, int, int)> keyboardVoid; // FOR USER DEFINE
   function<void(int, int, int)> specialVoid;            // FOR USER DEFINE

   Engine()
   {
      instance = this;
   }

   static void game(int arg)
   {
      if (instance && instance->gameVoid)
         instance->gameVoid();
      glutTimerFunc(timeGameStep, game, 0);
   }

   static void keyboard(unsigned char key, int x, int y)
   {
      if (instance && instance->keyboardVoid)
         instance->keyboardVoid(key, x, y);
   }

   static void mouse(int button, int state, int x, int y)
   {
      if (instance && instance->mouseVoid)
         instance->mouseVoid(button, state, x, y);
   }

   static void motion(int x, int y)
   {
      if (instance && instance->motionVoid)
         instance->motionVoid(x, y);
   }
   static void passiveMotion(int x, int y)
   {
      if (instance && instance->passiveMotionVoid)
         instance->passiveMotionVoid(x, y);
   }

   static void special(int key, int x, int y)
   {
      if (instance && instance->specialVoid)
         instance->specialVoid(key, x, y);
   }

   static void reshape(int t_width, int t_height)
   {
      width = t_width;
      height = t_height;
   }

   static void display()
   {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      canvas.apply(width,height,showFPS);

      glutSwapBuffers();
      glutPostRedisplay();
   }

   void start(int argc, char *argv[])
   {
      // DEFINE WINDOWS CONFIGS
      glutInit(&argc, argv);
      glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

      glutInitWindowSize(width, height);

      glutCreateWindow(windowName.c_str());

      if (fullscreen)
      {
         glutFullScreen();
      }
      glutDisplayFunc(display);             // FPS RENDER LOOP
      glutTimerFunc(timeGameStep, game, 0); // GAME STEP LOOP
      glutKeyboardFunc(keyboard);           // INPUT
      glutMouseFunc(mouse);                 // INPUT
      glutSpecialFunc(special);             // INPUT
      glutMotionFunc(motion);               // INPUT
      glutPassiveMotionFunc(passiveMotion); // INPUT
      glutReshapeFunc(reshape);             // INPUT
      glutMainLoop();
   }

   void reload()
   {
      if (fullscreen)
      {
         glutFullScreen();
      }
      else
      {
         glutReshapeWindow(width, height);
      }
   }
};

// game.cpp --------------------------------------------------------------------------

// void mainGameVoid()
// {
//    Loader loader;
//    Configs configs = loader.loadConfig();
//    if (configs == n_configs)
//    {
//       loader.saveConfig(configs); // SAVE BECAUSE DOESNT EXIST THE FILE
//    }
//    else
//    {
//       engine.configs = configs; // IS DIFFERENT FROM NORMAL, RELOAD
//       engine.reload();
//    }

//    // TO DO
//    engine.keyboardVoid = [](unsigned char key, int x, int y)
//    {
//       cout << "key: " << key << " Mouse Position: (" << x << "," << y << ")." << endl;
//    };

//    engine.set2D();
//    engine.color(1.0f, i, i);
// };

// void mainDisplayVoid()
// {
//    engine.text(engine.configs.widthWindow / 2, engine.configs.heightWindow / 2 + 24, "By: Matheus Ernesto", 0);
// };

Engine engine;
Configs configs;
int gameMenu = 0;

int main(int argc, char *argv[])
{
   Loader loader;
   Configs t_configs = loader.loadConfig();
   if (configs == t_configs)
   {
      loader.saveConfig(configs); // SAVE BECAUSE DOESNT EXIST THE FILE
   }
   else
   {
      configs = t_configs; // IS DIFFERENT FROM NORMAL
   }

   engine.fullscreen = configs.fullscreen;
   engine.width = configs.widthWindow;
   engine.height = configs.heightWindow;
   engine.showFPS = configs.showFPS;

   // Game configs
   engine.timeGameStep = 16; // 60FPS for game steps
   engine.windowName = "Metal Himiko - V0.0";

   // Game code

   // LOOP WITH STEP
   engine.gameVoid = []()
   {
      switch (gameMenu)
      {
      case 0:
         // TO DO
         engine.canvas.text(400,300,"Metal Himiko",0,1.0f,1.0f,1.0f);
         break;
      default:
         break;
      }
   };

   // LOOP WITH INPUTFUNC
   engine.keyboardVoid = [](unsigned char key, int x, int y)
   {
      if (key == 27)
      {
         exit(0);
      }
   };

   engine.mouseVoid = [](int button, int state, int x, int y)
   {
      
   };
   engine.specialVoid = [](int key, int x, int y)
   {
      
   };
   engine.motionVoid = [](int x, int y)
   {

   };
   engine.passiveMotionVoid = [](int x, int y)
   {

   };

   engine.start(argc, argv);
   return 0;
}
