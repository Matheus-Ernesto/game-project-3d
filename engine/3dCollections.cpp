#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <GL/glu.h>
#include <GL/gl.h>

using namespace std;

class Model
{
};

class Texture
{
public:
    float r = 1.f, g = 1.f, b = 1.f, a = 1.f;

    static constexpr int WHITEBONE = 0;
    static constexpr int GREY = 1;
    static constexpr int RED = 2;
    static constexpr int BLUE = 3;
    static constexpr int GREEN = 4;
    static constexpr int GLASSBLUE = 5;
    static constexpr int OTHER = 6;

    void setColor(int type)
    {

        switch (type)
        {
        case WHITEBONE:
            r = 0.8f;
            g = 0.8f;
            b = 0.9f;
            a = 1.f;
            break;

        case GREY:
            r = 0.2f;
            g = 0.2f;
            b = 0.2f;
            a = 1.f;
            break;

        case RED:
            r = 1.f;
            g = 0.f;
            b = 0.f;
            a = 1.f;
            break;

        case BLUE:
            r = 0.f;
            g = 0.f;
            b = 1.f;
            a = 1.f;
            break;

        case GREEN:
            r = 0.f;
            g = 1.f;
            b = 0.f;
            a = 1.f;
            break;

        case GLASSBLUE:
            r = 0.2f;
            g = 0.6f;
            b = 1.f;
            a = 0.5f;
            break;

        case OTHER:
            r = 0.5f;
            g = 0.5f;
            b = 0.5f;
            a = 1.f;
            break;

        default:
            r = 1.f;
            g = 1.f;
            b = 1.f;
            a = 1.f;
            break;
        }
    }

    vector<float> getColor() const
    {
        return {r, g, b, a};
    }
};

class Object3D
{
public:
    float pos_x = 0.f, pos_y = 0.f, pos_z = 0.f;
    float rot_x = 0.f, rot_y = 0.f, rot_z = 0.f;
    float sca_x = 1.f, sca_y = 1.f, sca_z = 1.f;

    Texture texture;
    bool visible = true;
    string name = "";

    Model model;

    int type = 0;
    static constexpr int CUBE = 0;
    static constexpr int MODEL = 1;

    void draw()
    {
        GLfloat mat_diffuse[] = {texture.r, texture.g, texture.b, texture.a};
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

        GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);

        GLfloat mat_shininess[] = {20.0f}; // Brilho
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
        switch (type)
        {
        case CUBE:
            glPushMatrix();
            glTranslatef(pos_x, pos_y, pos_z);

            glRotatef(rot_x, 1.f, 0.f, 0.f);
            glRotatef(rot_y, 0.f, 1.f, 0.f);
            glRotatef(rot_z, 0.f, 0.f, 1.f);

            glScalef(sca_x, sca_y, sca_z);

            glBegin(GL_QUADS);

            glColor3f(texture.r, texture.g, texture.b);

            // Face da frente (normal no eixo +z)
            glNormal3f(0.f, 0.f, 1.f); // Normal apontando para o eixo Z positivo
            glVertex3f(-1.f, -1.f, 1.f);
            glVertex3f(1.f, -1.f, 1.f);
            glVertex3f(1.f, 1.f, 1.f);
            glVertex3f(-1.f, 1.f, 1.f);

            // Face de trás (normal no eixo -z)
            glNormal3f(0.f, 0.f, -1.f); // Normal apontando para o eixo Z negativo
            glVertex3f(-1.f, -1.f, -1.f);
            glVertex3f(1.f, -1.f, -1.f);
            glVertex3f(1.f, 1.f, -1.f);
            glVertex3f(-1.f, 1.f, -1.f);

            // Face do lado esquerdo (normal no eixo -x)
            glNormal3f(-1.f, 0.f, 0.f); // Normal apontando para o eixo X negativo
            glVertex3f(-1.f, -1.f, 1.f);
            glVertex3f(-1.f, 1.f, 1.f);
            glVertex3f(-1.f, 1.f, -1.f);
            glVertex3f(-1.f, -1.f, -1.f);

            // Face do lado direito (normal no eixo +x)
            glNormal3f(1.f, 0.f, 0.f); // Normal apontando para o eixo X positivo
            glVertex3f(1.f, -1.f, -1.f);
            glVertex3f(1.f, 1.f, -1.f);
            glVertex3f(1.f, 1.f, 1.f);
            glVertex3f(1.f, -1.f, 1.f);

            // Face superior (normal no eixo +y)
            glNormal3f(0.f, 1.f, 0.f); // Normal apontando para o eixo Y positivo
            glVertex3f(-1.f, 1.f, -1.f);
            glVertex3f(-1.f, 1.f, 1.f);
            glVertex3f(1.f, 1.f, 1.f);
            glVertex3f(1.f, 1.f, -1.f);

            // Face inferior (normal no eixo -y)
            glNormal3f(0.f, -1.f, 0.f); // Normal apontando para o eixo Y negativo
            glVertex3f(-1.f, -1.f, -1.f);
            glVertex3f(-1.f, -1.f, 1.f);
            glVertex3f(1.f, -1.f, 1.f);
            glVertex3f(1.f, -1.f, -1.f);
 
            glEnd();

            glPopMatrix();
            break;
        case MODEL:
            break;
        default:
            break;
        }
    }
};

class Camera
{
public:
    float rotationX, rotationY, rotationZ;
    float x, y, z;
    float fov, limitNear, limitFar;
};

class Scene
{
public:
    vector<Object3D> objects3d;

    void add(Object3D obj)
    {
        objects3d.push_back(obj);
    }

    void draw()
    {
        glEnable(GL_LIGHTING); // Habilita o sistema de iluminação
        glEnable(GL_LIGHT0);   // Habilita a luz 0

        GLfloat light_position[] = {0.0f, 3.0f, 2.0f, 1.0f}; // Posição da luz
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);

        GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f}; // Cor da luz (branca)
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

        GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f}; // Reflexão especular
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

        GLfloat light_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f}; // Luz ambiente (suave)
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);

        for (auto &obj : objects3d)
        {
            if (obj.visible)
                obj.draw();
        }
    }

    Object3D &getObject3D(string name)
    {
        for (auto &obj : objects3d)
        {
            if (obj.name == name)
                return obj;
        }
        static Object3D invalidObject;
        cout << "err Obj " << name << "not found\n";
        return invalidObject;
    }
};

class World
{
public:
    float colorBackgroud_r, colorBackgroud_g, colorBackgroud_b;
    Scene scene;

    Camera camera;

    World()
    {
        camera.x = 0.f;
        camera.y = 0.f;
        camera.z = -10.f;
        camera.rotationX = 60.f;
        camera.rotationY = 30.f;
        camera.rotationZ = 0.f;
        camera.fov = 70.f;
        camera.limitNear = 0.1f;
        camera.limitFar = 1000.f;
        colorBackgroud_r = 0.f;
        colorBackgroud_g = 0.f;
        colorBackgroud_b = 0.f;
    }

    void setBackgroundColor(float r, float g, float b)
    {
        colorBackgroud_r = r;
        colorBackgroud_g = g;
        colorBackgroud_b = b;
    }

    void drawBackground()
    {
        glClearColor(colorBackgroud_r, colorBackgroud_g, colorBackgroud_b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void draw()
    {
        scene.draw();
    };
};

class Canvas3D
{
public:
    World world;
    bool enable3D;

    void draw(sf::RenderWindow &window)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(world.camera.fov, window.getSize().x / window.getSize().y, world.camera.limitNear, world.camera.limitFar);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glTranslatef(world.camera.x, world.camera.y, world.camera.z);
        glRotatef(world.camera.rotationX, 1.f, 0.f, 0.f);
        glRotatef(world.camera.rotationY, 0.f, 1.f, 0.f);
        glRotatef(world.camera.rotationZ, 0.f, 0.f, 1.f);

        // world.drawBackground();
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glEnable(GL_DEPTH_TEST);
        world.draw();
    };

    void apply(sf::RenderWindow &window, int width, int height, bool showFps)
    {
        draw(window);
    };

    void setup(sf::RenderWindow &window)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CW);
        glEnable(GL_DEPTH_TEST);
    }
};