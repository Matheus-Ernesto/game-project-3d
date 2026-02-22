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
    // Posição
    float pos_x = 0.0f;
    float pos_y = 5.0f;
    float pos_z = 5.0f;

    // Cores da luz
    float diffuse_r = 1.0f; // Vermelho da luz difusa
    float diffuse_g = 1.0f; // Verde da luz difusa
    float diffuse_b = 1.0f; // Azul da luz difusa
    float diffuse_a = 1.0f; // Alpha da luz difusa

    float ambient_r = 0.2f; // Vermelho da luz ambiente
    float ambient_g = 0.2f; // Verde da luz ambiente
    float ambient_b = 0.2f; // Azul da luz ambiente
    float ambient_a = 1.0f; // Alpha da luz ambiente

    float specular_r = 1.0f; // Vermelho da luz especular
    float specular_g = 1.0f; // Verde da luz especular
    float specular_b = 1.0f; // Azul da luz especular
    float specular_a = 1.0f; // Alpha da luz especular

    // Atenuação
    float constant_attenuation = 1.0f;
    float linear_attenuation = 0.05f;
    float quadratic_attenuation = 0.01f;

    // Tipo de luz: 0 = pontual, 1 = direcional, 2 = spot
    int type = 0;

    // Direção para luz direcional ou spot
    float dir_x = 0.0f;
    float dir_y = -1.0f;
    float dir_z = 0.0f;

    // Parâmetros de spot
    float spot_cutoff = 45.0f;  // Ângulo de corte do spot (0-180)
    float spot_exponent = 0.0f; // Concentração do spot

    // Nome da luz (para referência)
    std::string name = "default_light";

    // Se a luz está ativa
    bool enabled = true;

    // Métodos auxiliares para configurar cores
    void setDiffuse(float r, float g, float b, float a = 1.0f)
    {
        diffuse_r = r;
        diffuse_g = g;
        diffuse_b = b;
        diffuse_a = a;
    }

    void setAmbient(float r, float g, float b, float a = 1.0f)
    {
        ambient_r = r;
        ambient_g = g;
        ambient_b = b;
        ambient_a = a;
    }

    void setSpecular(float r, float g, float b, float a = 1.0f)
    {
        specular_r = r;
        specular_g = g;
        specular_b = b;
        specular_a = a;
    }

    void setPosition(float x, float y, float z)
    {
        pos_x = x;
        pos_y = y;
        pos_z = z;
    }

    void setDirection(float x, float y, float z)
    {
        dir_x = x;
        dir_y = y;
        dir_z = z;
    }

    void setAttenuation(float constant, float linear, float quadratic)
    {
        constant_attenuation = constant;
        linear_attenuation = linear;
        quadratic_attenuation = quadratic;
    }

    void setSpot(float cutoff, float exponent)
    {
        spot_cutoff = cutoff;
        spot_exponent = exponent;
    }
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
    vector<Light> lights;

    void add(Object3D obj)
    {
        objects3d.push_back(obj);
    }

    void add(Light light)
    {
        lights.push_back(light);
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
    Light &getLight(string name)
    {
        for (auto &obj : lights)
        {
            if (obj.name == name)
                return obj;
        }
        static Light invalidObject;
        cout << "err Light " << name << " not found\n";
        return invalidObject;
    }

    void clearAll()
    {
        objects3d.clear();
        lights.clear();
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