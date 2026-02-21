#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include <cmath>

#include <GL/glu.h>
#include <GL/gl.h>

using namespace std;

class Canvas3D
{
public:
    World world;
    bool enable3D;

    struct TextureGL
    {
        GLuint id;
        std::string path;
        int width;
        int height;
        int type;
    };

    std::vector<TextureGL> canvasTextures;

    GLuint loadTextureToGL(const std::string &filename, int type)
    {
        sf::Image image;
        if (!image.loadFromFile(filename))
        {
            printf("Erro ao carregar: %s\n", filename.c_str());
            return 0;
        }

        image.flipVertically();

        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Configura parâmetros da textura - use GL_LINEAR em vez de GL_NEAREST
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtro linear para minimização
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtro linear para magnificação

        // Envia a imagem para o OpenGL
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());

        // Verifica se houve erro
        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            printf("Erro OpenGL ao carregar textura: %d\n", error);
        }

        return textureID;
    }

    // Função para carregar textura e retornar índice
    int loadCanvasTexture(const std::string &path, int type = 0)
    {
        // Verifica se já existe
        for (size_t i = 0; i < canvasTextures.size(); i++)
        {
            if (canvasTextures[i].path == path && canvasTextures[i].type == type)
            {
                return i;
            }
        }

        GLuint glID = loadTextureToGL(path, type);
        if (glID == 0)
            return -1;

        TextureGL tex;
        tex.id = glID;
        tex.path = path;
        tex.type = type;

        sf::Image img;
        if (img.loadFromFile(path))
        {
            tex.width = img.getSize().x;
            tex.height = img.getSize().y;
        }

        canvasTextures.push_back(tex);
        return canvasTextures.size() - 1;
    }

    // Função para aplicar textura pelo índice
    void applyCanvasTexture(int textureIndex)
    {
        if (textureIndex >= 0 && textureIndex < canvasTextures.size())
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, canvasTextures[textureIndex].id);

            // Verifica se a textura está ativa
            GLint boundTexture;
            glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexture);
        }
        else
        {
            glDisable(GL_TEXTURE_2D);
            printf("Desabilitando textura\n");
        }
    }

    // Função para aplicar textura baseado no objeto
    void applyObjectTexture(const Texture &objTexture)
    {
        if (objTexture.hasTexture())
        {
            int index = loadCanvasTexture(objTexture.path, objTexture.textureType);
            applyCanvasTexture(index);
        }
        else
        {
            glDisable(GL_TEXTURE_2D);
        }
    }

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
            applyObjectTexture(obj.texture);

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
                glNormal3f(0.0f, 0.0f, 1.0f);
                if (obj.texture.hasTexture())
                {
                    glTexCoord2f(0.0f, 0.0f);
                    glVertex3f(-1.0f, -1.0f, 1.0f);
                    glTexCoord2f(1.0f, 0.0f);
                    glVertex3f(1.0f, -1.0f, 1.0f);
                    glTexCoord2f(1.0f, 1.0f);
                    glVertex3f(1.0f, 1.0f, 1.0f);
                    glTexCoord2f(0.0f, 1.0f);
                    glVertex3f(-1.0f, 1.0f, 1.0f);
                }
                else
                {
                    glVertex3f(-1.0f, -1.0f, 1.0f);
                    glVertex3f(1.0f, -1.0f, 1.0f);
                    glVertex3f(1.0f, 1.0f, 1.0f);
                    glVertex3f(-1.0f, 1.0f, 1.0f);
                }

                // Face traseira (normal para -Z)
                glNormal3f(0.0f, 0.0f, -1.0f);
                if (obj.texture.hasTexture())
                {
                    glTexCoord2f(0.0f, 0.0f);
                    glVertex3f(-1.0f, -1.0f, -1.0f);
                    glTexCoord2f(1.0f, 0.0f);
                    glVertex3f(-1.0f, 1.0f, -1.0f);
                    glTexCoord2f(1.0f, 1.0f);
                    glVertex3f(1.0f, 1.0f, -1.0f);
                    glTexCoord2f(0.0f, 1.0f);
                    glVertex3f(1.0f, -1.0f, -1.0f);
                }
                else
                {
                    glVertex3f(-1.0f, -1.0f, -1.0f);
                    glVertex3f(-1.0f, 1.0f, -1.0f);
                    glVertex3f(1.0f, 1.0f, -1.0f);
                    glVertex3f(1.0f, -1.0f, -1.0f);
                }

                // Face superior (normal para +Y)
                glNormal3f(0.0f, 1.0f, 0.0f);
                if (obj.texture.hasTexture())
                {
                    glTexCoord2f(0.0f, 0.0f);
                    glVertex3f(-1.0f, 1.0f, -1.0f);
                    glTexCoord2f(1.0f, 0.0f);
                    glVertex3f(-1.0f, 1.0f, 1.0f);
                    glTexCoord2f(1.0f, 1.0f);
                    glVertex3f(1.0f, 1.0f, 1.0f);
                    glTexCoord2f(0.0f, 1.0f);
                    glVertex3f(1.0f, 1.0f, -1.0f);
                }
                else
                {
                    glVertex3f(-1.0f, 1.0f, -1.0f);
                    glVertex3f(-1.0f, 1.0f, 1.0f);
                    glVertex3f(1.0f, 1.0f, 1.0f);
                    glVertex3f(1.0f, 1.0f, -1.0f);
                }

                // Face inferior (normal para -Y)
                glNormal3f(0.0f, -1.0f, 0.0f);
                if (obj.texture.hasTexture())
                {
                    glTexCoord2f(0.0f, 0.0f);
                    glVertex3f(-1.0f, -1.0f, -1.0f);
                    glTexCoord2f(1.0f, 0.0f);
                    glVertex3f(1.0f, -1.0f, -1.0f);
                    glTexCoord2f(1.0f, 1.0f);
                    glVertex3f(1.0f, -1.0f, 1.0f);
                    glTexCoord2f(0.0f, 1.0f);
                    glVertex3f(-1.0f, -1.0f, 1.0f);
                }
                else
                {
                    glVertex3f(-1.0f, -1.0f, -1.0f);
                    glVertex3f(1.0f, -1.0f, -1.0f);
                    glVertex3f(1.0f, -1.0f, 1.0f);
                    glVertex3f(-1.0f, -1.0f, 1.0f);
                }

                // Face esquerda (normal para -X)
                glNormal3f(-1.0f, 0.0f, 0.0f);
                if (obj.texture.hasTexture())
                {
                    glTexCoord2f(0.0f, 0.0f);
                    glVertex3f(-1.0f, -1.0f, -1.0f);
                    glTexCoord2f(1.0f, 0.0f);
                    glVertex3f(-1.0f, -1.0f, 1.0f);
                    glTexCoord2f(1.0f, 1.0f);
                    glVertex3f(-1.0f, 1.0f, 1.0f);
                    glTexCoord2f(0.0f, 1.0f);
                    glVertex3f(-1.0f, 1.0f, -1.0f);
                }
                else
                {
                    glVertex3f(-1.0f, -1.0f, -1.0f);
                    glVertex3f(-1.0f, -1.0f, 1.0f);
                    glVertex3f(-1.0f, 1.0f, 1.0f);
                    glVertex3f(-1.0f, 1.0f, -1.0f);
                }

                // Face direita (normal para +X)
                glNormal3f(1.0f, 0.0f, 0.0f);
                if (obj.texture.hasTexture())
                {
                    glTexCoord2f(0.0f, 0.0f);
                    glVertex3f(1.0f, -1.0f, -1.0f);
                    glTexCoord2f(1.0f, 0.0f);
                    glVertex3f(1.0f, 1.0f, -1.0f);
                    glTexCoord2f(1.0f, 1.0f);
                    glVertex3f(1.0f, 1.0f, 1.0f);
                    glTexCoord2f(0.0f, 1.0f);
                    glVertex3f(1.0f, -1.0f, 1.0f);
                }
                else
                {
                    glVertex3f(1.0f, -1.0f, -1.0f);
                    glVertex3f(1.0f, 1.0f, -1.0f);
                    glVertex3f(1.0f, 1.0f, 1.0f);
                    glVertex3f(1.0f, -1.0f, 1.0f);
                }

                glEnd();
            }
            else if (obj.type >= 2 && obj.type <= 5)
            {
                // Número de segmentos - ajuste para controlar o nível de detalhe
                int stacks = 4 * obj.type; // Número de divisões verticais (paralelos) - 16 * 32 = 512 triângulos (aproximadamente)
                int slices = 8 * obj.type; // Número de divisões horizontais (meridianos)

                float radius = 1.0f; // Raio da esfera

                glBegin(GL_TRIANGLES);

                for (int i = 0; i < stacks; i++)
                {
                    // Ângulos vertical (latitude) - de -90° a +90°
                    float theta1 = (float(i) / stacks) * M_PI - M_PI / 2; // -90° a +90°
                    float theta2 = (float(i + 1) / stacks) * M_PI - M_PI / 2;

                    for (int j = 0; j < slices; j++)
                    {
                        // Ângulos horizontal (longitude) - de 0° a 360°
                        float phi1 = (float(j) / slices) * 2 * M_PI;
                        float phi2 = (float(j + 1) / slices) * 2 * M_PI;

                        // Coordenadas dos 4 vértices do quadrilátero
                        float x11 = radius * cos(theta1) * cos(phi1);
                        float y11 = radius * sin(theta1);
                        float z11 = radius * cos(theta1) * sin(phi1);

                        float x12 = radius * cos(theta1) * cos(phi2);
                        float y12 = radius * sin(theta1);
                        float z12 = radius * cos(theta1) * sin(phi2);

                        float x21 = radius * cos(theta2) * cos(phi1);
                        float y21 = radius * sin(theta2);
                        float z21 = radius * cos(theta2) * sin(phi1);

                        float x22 = radius * cos(theta2) * cos(phi2);
                        float y22 = radius * sin(theta2);
                        float z22 = radius * cos(theta2) * sin(phi2);

                        // Normal é o próprio vértice normalizado (para uma esfera centrada na origem)
                        float nx, ny, nz;
                        float length;

                        // Primeiro triângulo (superior-esquerdo, superior-direito, inferior-esquerdo)
                        // Vértice 1 (theta1, phi1)
                        nx = x11;
                        ny = y11;
                        nz = z11;
                        length = sqrt(nx * nx + ny * ny + nz * nz);
                        glNormal3f(nx / length, ny / length, nz / length);
                        glVertex3f(x11, y11, z11);

                        // Vértice 2 (theta1, phi2)
                        nx = x12;
                        ny = y12;
                        nz = z12;
                        length = sqrt(nx * nx + ny * ny + nz * nz);
                        glNormal3f(nx / length, ny / length, nz / length);
                        glVertex3f(x12, y12, z12);

                        // Vértice 3 (theta2, phi1)
                        nx = x21;
                        ny = y21;
                        nz = z21;
                        length = sqrt(nx * nx + ny * ny + nz * nz);
                        glNormal3f(nx / length, ny / length, nz / length);
                        glVertex3f(x21, y21, z21);

                        // Segundo triângulo (superior-direito, inferior-direito, inferior-esquerdo)
                        // Vértice 2 (theta1, phi2)
                        nx = x12;
                        ny = y12;
                        nz = z12;
                        length = sqrt(nx * nx + ny * ny + nz * nz);
                        glNormal3f(nx / length, ny / length, nz / length);
                        glVertex3f(x12, y12, z12);

                        // Vértice 3 (theta2, phi2)
                        nx = x22;
                        ny = y22;
                        nz = z22;
                        length = sqrt(nx * nx + ny * ny + nz * nz);
                        glNormal3f(nx / length, ny / length, nz / length);
                        glVertex3f(x22, y22, z22);

                        // Vértice 1 (theta2, phi1)
                        nx = x21;
                        ny = y21;
                        nz = z21;
                        length = sqrt(nx * nx + ny * ny + nz * nz);
                        glNormal3f(nx / length, ny / length, nz / length);
                        glVertex3f(x21, y21, z21);
                    }
                }

                glEnd();
            }

            glPopMatrix();
            glDisable(GL_TEXTURE_2D);
        }
    }

    void apply(sf::RenderWindow &window, int width, int height, bool showFps)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glEnable(GL_DEPTH_TEST);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(world.camera.fov, window.getSize().x / (float)window.getSize().y, world.camera.limitNear, world.camera.limitFar);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef(world.camera.rotationX, 1.f, 0.f, 0.f);
        glRotatef(world.camera.rotationY, 0.f, 1.f, 0.f);
        glRotatef(world.camera.rotationZ, 0.f, 0.f, 1.f);
        glTranslatef(world.camera.x, world.camera.y, world.camera.z);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        draw(window);
    }

    void update()
    {
    }
};