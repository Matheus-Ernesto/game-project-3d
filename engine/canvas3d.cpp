#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include <GL/glu.h>
#include <GL/gl.h>

using namespace std;

class Canvas3D
{
public:
    World world;
    bool enable3D;

    void draw(sf::RenderWindow &window)
    {
        glEnable(GL_LIGHTING); // Habilita o sistema de iluminação
        glEnable(GL_LIGHT0);   // Habilita a luz 0

        GLfloat light_position[] = {5.0f, 5.0f, 3.0f, 1.f}; // Luz mais central
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);

        GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f}; // Cor da luz (branca)
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

        GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f}; // Reflexão especular
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

        GLfloat light_ambient[] = {0.3f, 0.3f, 0.3f, 1.0f}; // Luz ambiente (suave)
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);

        for (auto &obj : world.scene.objects3d)
        {
            GLfloat mat_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);

            GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);

            GLfloat mat_shininess[] = {30.0f}; // Brilho
            glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

            glPushMatrix();

            glTranslatef(obj.pos_x, obj.pos_y, obj.pos_z);
            glScalef(obj.sca_x, obj.sca_y, obj.sca_z);
            glRotatef(obj.rot_x, 1.0, 0.0, 0.0);
            glRotatef(obj.rot_y, 0.0, 1.0, 0.0);
            glRotatef(obj.rot_z, 0.0, 0.0, 1.0);

            switch (obj.type)
            {
            case obj.CUBE:
                glBegin(GL_QUADS);

                // Face frontal (vermelha)
                glColor3f(obj.texture.r, obj.texture.g, obj.texture.b);
                glVertex3f(-1.0f, -1.0f, 1.0f);
                glVertex3f(1.0f, -1.0f, 1.0f);
                glVertex3f(1.0f, 1.0f, 1.0f);
                glVertex3f(-1.0f, 1.0f, 1.0f);

                // Face traseira (verde)
                glColor3f(obj.texture.r, obj.texture.g, obj.texture.b);
                glVertex3f(-1.0f, -1.0f, -1.0f);
                glVertex3f(-1.0f, 1.0f, -1.0f);
                glVertex3f(1.0f, 1.0f, -1.0f);
                glVertex3f(1.0f, -1.0f, -1.0f);

                // Face superior (azul)
                glColor3f(obj.texture.r, obj.texture.g, obj.texture.b);
                glVertex3f(-1.0f, 1.0f, -1.0f);
                glVertex3f(-1.0f, 1.0f, 1.0f);
                glVertex3f(1.0f, 1.0f, 1.0f);
                glVertex3f(1.0f, 1.0f, -1.0f);

                // Face inferior (amarela)
                glColor3f(obj.texture.r, obj.texture.g, obj.texture.b);
                glVertex3f(-1.0f, -1.0f, -1.0f);
                glVertex3f(1.0f, -1.0f, -1.0f);
                glVertex3f(1.0f, -1.0f, 1.0f);
                glVertex3f(-1.0f, -1.0f, 1.0f);

                // Face esquerda (ciano)
                glColor3f(obj.texture.r, obj.texture.g, obj.texture.b);
                glVertex3f(-1.0f, -1.0f, -1.0f);
                glVertex3f(-1.0f, -1.0f, 1.0f);
                glVertex3f(-1.0f, 1.0f, 1.0f);
                glVertex3f(-1.0f, 1.0f, -1.0f);

                // Face direita (magenta)
                glColor3f(obj.texture.r, obj.texture.g, obj.texture.b);
                glVertex3f(1.0f, -1.0f, -1.0f);
                glVertex3f(1.0f, 1.0f, -1.0f);
                glVertex3f(1.0f, 1.0f, 1.0f);
                glVertex3f(1.0f, -1.0f, 1.0f);

                glEnd();

                glPopMatrix();
                break;

            default:
                break;
            }
        }
    }

    void apply(sf::RenderWindow &window, int width, int height, bool showFps)
    {
        // Limpa o buffer de cor e profundidade (necessário para 3D)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Cor de fundo preta
        glEnable(GL_DEPTH_TEST);              // Ativa o teste de profundidade para 3D

        // Configuração de projeção 3D
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(world.camera.fov, window.getSize().x / (float)window.getSize().y, world.camera.limitNear, world.camera.limitFar);

        // Configuração da visão (modelview)
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(world.camera.x, world.camera.y, world.camera.z);
        glRotatef(world.camera.rotationX, 1.f, 0.f, 0.f);
        glRotatef(world.camera.rotationY, 0.f, 1.f, 0.f);
        glRotatef(world.camera.rotationZ, 0.f, 0.f, 1.f);

        glEnable(GL_CULL_FACE); // Desenha apenas as faces frontais
        glCullFace(GL_BACK);

        draw(window);
    };
};