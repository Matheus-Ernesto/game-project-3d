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
        // Habilita o sistema de iluminação
        glEnable(GL_LIGHTING); // Habilita a iluminação global
        glEnable(GL_LIGHT0);   // Habilita a luz 0

        // Posição da luz (a posição em espaço homogêneo (x, y, z, w) - w=1 para luz pontual)
        GLfloat light_position[] = {5.0f, 5.0f, 3.0f, 1.0f};
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);

        // Propriedades da luz (difusa, especular, ambiente)
        GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f}; // Cor difusa (branca)
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

        GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f}; // Reflexão especular
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

        GLfloat light_ambient[] = {0.3f, 0.3f, 0.3f, 1.0f}; // Cor ambiente (luz suave)
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);

        // Atenuação da luz
        GLfloat attenuation[] = {1.0f, 0.05f, 0.01f}; // Atenuação de luz
        glLightfv(GL_LIGHT0, GL_CONSTANT_ATTENUATION, &attenuation[0]);
        glLightfv(GL_LIGHT0, GL_LINEAR_ATTENUATION, &attenuation[1]);
        glLightfv(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, &attenuation[2]);

        // Para cada objeto na cena, defina as propriedades do material e desenhe o objeto
        for (auto &obj : world.scene.objects3d)
        {
            // Propriedades do material do objeto (difusa, especular, brilho)
            GLfloat mat_diffuse[] = {obj.texture.r, obj.texture.g, obj.texture.b, 1.0f};
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);

            GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);

            GLfloat mat_shininess[] = {30.0f}; // Brilho do material
            glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

            glPushMatrix();

            // Transformações para posicionar, escalar e rotacionar o objeto
            glTranslatef(obj.pos_x, obj.pos_y, obj.pos_z);
            glScalef(obj.sca_x, obj.sca_y, obj.sca_z);
            glRotatef(obj.rot_x, 1.0, 0.0, 0.0);
            glRotatef(obj.rot_y, 0.0, 1.0, 0.0);
            glRotatef(obj.rot_z, 0.0, 0.0, 1.0);

            if (obj.type == obj.CUBE)
            {
                glBegin(GL_QUADS);

                // Face frontal (normal para +Z)
                glNormal3f(0.0f, 0.0f, 1.0f); // Normal para a face frontal
                glVertex3f(-1.0f, -1.0f, 1.0f);
                glVertex3f(1.0f, -1.0f, 1.0f);
                glVertex3f(1.0f, 1.0f, 1.0f);
                glVertex3f(-1.0f, 1.0f, 1.0f);

                // Face traseira (normal para -Z)
                glNormal3f(0.0f, 0.0f, -1.0f); // Normal para a face traseira
                glVertex3f(-1.0f, -1.0f, -1.0f);
                glVertex3f(-1.0f, 1.0f, -1.0f);
                glVertex3f(1.0f, 1.0f, -1.0f);
                glVertex3f(1.0f, -1.0f, -1.0f);

                // Face superior (normal para +Y)
                glNormal3f(0.0f, 1.0f, 0.0f); // Normal para a face superior
                glVertex3f(-1.0f, 1.0f, -1.0f);
                glVertex3f(-1.0f, 1.0f, 1.0f);
                glVertex3f(1.0f, 1.0f, 1.0f);
                glVertex3f(1.0f, 1.0f, -1.0f);

                // Face inferior (normal para -Y)
                glNormal3f(0.0f, -1.0f, 0.0f); // Normal para a face inferior
                glVertex3f(-1.0f, -1.0f, -1.0f);
                glVertex3f(1.0f, -1.0f, -1.0f);
                glVertex3f(1.0f, -1.0f, 1.0f);
                glVertex3f(-1.0f, -1.0f, 1.0f);

                // Face esquerda (normal para -X)
                glNormal3f(-1.0f, 0.0f, 0.0f); // Normal para a face esquerda
                glVertex3f(-1.0f, -1.0f, -1.0f);
                glVertex3f(-1.0f, -1.0f, 1.0f);
                glVertex3f(-1.0f, 1.0f, 1.0f);
                glVertex3f(-1.0f, 1.0f, -1.0f);

                // Face direita (normal para +X)
                glNormal3f(1.0f, 0.0f, 0.0f); // Normal para a face direita
                glVertex3f(1.0f, -1.0f, -1.0f);
                glVertex3f(1.0f, 1.0f, -1.0f);
                glVertex3f(1.0f, 1.0f, 1.0f);
                glVertex3f(1.0f, -1.0f, 1.0f);

                glEnd();
            }

            glPopMatrix();
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

        // Chama a função de desenho com iluminação ativada
        draw(window);
    }

    void update()
    {
    }
};