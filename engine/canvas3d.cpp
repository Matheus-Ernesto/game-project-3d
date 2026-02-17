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
        glDisable(GL_BLEND);     // Desabilita o blending
        glEnable(GL_DEPTH_TEST); // Habilita o teste de profundidade

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

        // Habilita o culling para não desenhar faces traseiras
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CW); // Se necessário, inverta para GL_CCW (contra-horário)

        // Desenhando o cubo face a face

        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.0f);

        glBegin(GL_QUADS);

        // Superior
        glColor3f(1.f, 1.f, 0.f);  // Amarelo
        glNormal3f(0.f, 1.f, 0.f); // Normal apontando para o eixo Y positivo
        glVertex3f(-1.f, 1.f, -1.f);
        glVertex3f(-1.f, 1.f, 1.f);
        glVertex3f(1.f, 1.f, 1.f);
        glVertex3f(1.f, 1.f, -1.f);

        // Frente
        glColor3f(1.f, 1.f, 1.f);  // Branco
        glNormal3f(0.f, 0.f, 1.f); // Normal apontando para o eixo Z positivo
        glVertex3f(-1.f, -1.f, 1.f);
        glVertex3f(1.f, -1.f, 1.f);
        glVertex3f(1.f, 1.f, 1.f);
        glVertex3f(-1.f, 1.f, 1.f);

        // Lado esquerdo
        glColor3f(0.f, 0.f, 1.f);   // Azul
        glNormal3f(-1.f, 0.f, 0.f); // Normal apontando para o eixo X negativo
        glVertex3f(-1.f, -1.f, 1.f);
        glVertex3f(-1.f, 1.f, 1.f);
        glVertex3f(-1.f, 1.f, -1.f);
        glVertex3f(-1.f, -1.f, -1.f);

        // Trás
        glColor3f(0.f, 1.f, 1.f);   // Ciano
        glNormal3f(0.f, 0.f, -1.f); // Normal apontando para o eixo Z negativo
        glVertex3f(-1.f, -1.f, -1.f);
        glVertex3f(1.f, -1.f, -1.f);
        glVertex3f(1.f, 1.f, -1.f);
        glVertex3f(-1.f, 1.f, -1.f);
        
        // Lado direito
        glColor3f(0.f, 1.f, 0.f);  // Verde
        glNormal3f(1.f, 0.f, 0.f); // Normal apontando para o eixo X positivo
        glVertex3f(1.f, -1.f, -1.f);
        glVertex3f(1.f, 1.f, -1.f);
        glVertex3f(1.f, 1.f, 1.f);
        glVertex3f(1.f, -1.f, 1.f);

        // Inferior
        glColor3f(1.f, 0.f, 0.f);   // Vermelho
        glNormal3f(0.f, -1.f, 0.f); // Normal apontando para o eixo Y negativo
        glVertex3f(-1.f, -1.f, -1.f);
        glVertex3f(-1.f, -1.f, 1.f);
        glVertex3f(1.f, -1.f, 1.f);
        glVertex3f(1.f, -1.f, -1.f);

        glEnd();
        glPopMatrix();
    }

    void apply(sf::RenderWindow &window, int width, int height, bool showFps)
    {
        draw(window);
    };
};