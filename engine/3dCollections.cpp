#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

using namespace std;

class Model
{
};

class Texture
{
public:
    // Dados da textura
    float r = 1.f, g = 1.f, b = 1.f, a = 1.f;
    std::string path = ""; // Caminho da textura (se vazio, é cor sólida)
    int textureType = 0;   // Tipo de organização (0 = repetir, 1 = esticar, etc)

    static constexpr int WHITEBONE = 0;
    static constexpr int GREY = 1;
    static constexpr int RED = 2;
    static constexpr int BLUE = 3;
    static constexpr int GREEN = 4;
    static constexpr int GLASSBLUE = 5;
    static constexpr int OTHER = 6;
    static constexpr int TEXTURE_PNG = 7;

    void setColor(int type)
    {
        // Limpa o caminho da textura (agora é cor sólida)
        path = "";

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
        case TEXTURE_PNG:
            // Para textura PNG, mantém a cor branca
            r = 1.f;
            g = 1.f;
            b = 1.f;
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

    void setTexture(const std::string &texturePath, int type = 0)
    {
        path = texturePath;
        textureType = type;
        // Para textura, usamos cor branca para não interferir
        r = 1.f;
        g = 1.f;
        b = 1.f;
        a = 1.f;
    }

    bool hasTexture() const
    {
        return !path.empty();
    }

    vector<float> getColor() const
    {
        return {r, g, b, a};
    }
};

class Light
{
public:
};

class Object3D
{
public:
    float pos_x = 0.f, pos_y = 0.f, pos_z = 0.f;
    float rot_x = 0.f, rot_y = 0.f, rot_z = 0.f;
    float sca_x = 1.f, sca_y = 1.f, sca_z = 1.f;

    Texture texture;
    Model model;

    bool visible = true;
    string name = "";

    int type = 0;

    static constexpr int CUBE = 0;
    static constexpr int MODEL = 1;
    static constexpr int SPHERE_SUB1 = 2;
    static constexpr int SPHERE_SUB2 = 3;
    static constexpr int SPHERE_SUB3 = 4;
    static constexpr int SPHERE_SUB4 = 5;
};

class Camera
{
public:
    float x = 0.f, y = 0.f, z = -10.f;
    float rotationX = 60.f, rotationY = 30.f, rotationZ = 0.f;
    float fov = 70.f, limitNear = 0.1f, limitFar = 1000.f;

    void moveLensDirection()
    {
    }
};

class Scene
{
public:
    vector<Object3D> objects3d;

    void add(Object3D obj)
    {
        objects3d.push_back(obj);
    }

    Object3D &getObject3D(string name)
    {
        for (auto &obj : objects3d)
        {
            if (obj.name == name)
                return obj;
        }
        static Object3D invalidObject;
        cout << "err Obj " << name << " not found\n";
        return invalidObject;
    }

    void clearAll()
    {
        objects3d.clear();
    }
};

class World
{
public:
    float colorBackgroud_r = 0.f, colorBackgroud_g = 0.f, colorBackgroud_b = 0.f;
    Scene scene;
    Camera camera;

    void setBackgroundColor(float r, float g, float b)
    {
        colorBackgroud_r = r;
        colorBackgroud_g = g;
        colorBackgroud_b = b;
    }
};