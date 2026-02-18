#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>

float x = 0.f;
float y = 0.f;
float z = 0.f;

void setupOpenGL()
{
    // Inicializando o OpenGL
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Cor de fundo preta
    glEnable(GL_DEPTH_TEST);              // Habilita o teste de profundidade para 3D
}

void drawCube()
{
    // Desenha um cubo com faces coloridas
    glTranslatef(0.0, 0.0, -2.0);
    glRotatef(x, 1.0, 0.0, 0.0);
    glRotatef(y, 0.0, 1.0, 0.0);
    glRotatef(z, 0.0, 0.0, 1.0);

    glBegin(GL_QUADS);

    // Face frontal (vermelha)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    // Face traseira (verde)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    // Face superior (azul)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    // Face inferior (amarela)
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    // Face esquerda (ciano)
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    // Face direita (magenta)
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    glEnd();
}

int main()
{
    // Criação da janela SFML
    sf::RenderWindow window(sf::VideoMode(800, 600), "Cubo 3D com SFML", sf::Style::Close, sf::ContextSettings(24));

    // Configurações do OpenGL
    window.setActive(true);
    setupOpenGL();

    // Loop principal
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();

        }

        x += 0.01;
        y += 0.01;

        // Limpar o buffer de cor e profundidade
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Configura a projeção
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glFrustum(-1.0f, 1.0f, -1.0f, 1.0f, 2.0f, 10.0f);
        glTranslatef(0.0f, 0.0f, -5.0f);

        // Configura a visão do modelo
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glPushMatrix();

        drawCube();
        
        glPopMatrix();

        // Trocar buffers (double buffering)
        window.display();
    }

    return 0;
}
