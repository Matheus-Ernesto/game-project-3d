#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <string>
#include <vector>
#include <memory>

using namespace std;


namespace Collections3d{
    class Texture
    {
    public:
        // Dados da textura
        float r = 1.f, g = 1.f, b = 1.f, a = 1.f;
        string path = ""; // Caminho da textura (se vazio, é cor sólida)
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
    
        void setTexture(const string &texturePath, int type = 0)
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
        string name = "default_light";
    
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
    
    
    class Mesh {
    protected:
        int typeFace = 0;
        
    public:
        virtual ~Mesh() = default;
        
        virtual void getMesh() = 0;
        virtual vector<v3f> getVertices() = 0;
        virtual vector<unsigned int> getIndices() = 0;
    };
    
    class Transform {
    public:
        v3f position;
        v3f scale = v3f{1.f, 1.f, 1.f};
        v3f rotation;
        v4f deltaVel;
    };
    
    class Arc : public Mesh {
    protected:
        float radius = 1.f;
        float startAngle = 0.f;
        float endAngle = 360.f;
        v3f center;
        
    public:
        void getMesh() override {
            typeFace = 1;
        }
        
        vector<v3f> getVertices() override {
            vector<v3f> vertices;
            return vertices;
        }
        
        vector<unsigned int> getIndices() override {
            vector<unsigned int> indices;
            return indices;
        }
    };
    
    class Cylinder : public Arc {
    private:
        float height = 2.f;
        
    public:
        void getMesh() override {
            typeFace = 2;
        }
        
        vector<v3f> getVertices() override {
            vector<v3f> vertices;
            return vertices;
        }
        
        vector<unsigned int> getIndices() override {
            vector<unsigned int> indices;
            return indices;
        }
    };
    
    class Sphere : public Mesh {
    private:
        float radius = 1.f;
        int resolution = 16;
        v3f center;
        
    public:
        void setResolution(int res) { resolution = res; }
        int getResolution() { return resolution; }
        
        void getMesh() override {
            typeFace = 3;
        }
        
        vector<v3f> getVertices() override {
            vector<v3f> vertices;
            return vertices;
        }
        
        vector<unsigned int> getIndices() override {
            vector<unsigned int> indices;
            return indices;
        }
    };
    
    class Text : public Mesh {
    private:
        string content;
        float size = 1.f;
        v3f position;
        
    public:
        void setText(const string& text) { content = text; }
        string getText() { return content; }
        
        void getMesh() override {
            typeFace = 4;
        }
        
        vector<v3f> getVertices() override {
            vector<v3f> vertices;
            return vertices;
        }
        
        vector<unsigned int> getIndices() override {
            vector<unsigned int> indices;
            return indices;
        }
    };
    
    class Triangle : public Mesh {
    private:
        v3f p1;
        v3f p2;
        v3f p3;
        
    public:
        void getMesh() override {
            typeFace = 5;
        }
        
        vector<v3f> getVertices() override {
            vector<v3f> vertices;
            vertices.push_back(p1);
            vertices.push_back(p2);
            vertices.push_back(p3);
            return vertices;
        }
        
        vector<unsigned int> getIndices() override {
            vector<unsigned int> indices;
            indices.push_back(0);
            indices.push_back(1);
            indices.push_back(2);
            return indices;
        }
    };
    
    class Plane : public Mesh {
    private:
        float width = 1.f;
        float height = 1.f;
        v3f center;
        
    public:
        void getMesh() override {
            typeFace = 6;
        }
        
        vector<v3f> getVertices() override {
            vector<v3f> vertices;
            return vertices;
        }
        
        vector<unsigned int> getIndices() override {
            vector<unsigned int> indices;
            return indices;
        }
    };
    
    class Cube : public Mesh {
    private:
        float width = 1.f;
        float height = 1.f;
        
    public:
        void getMesh() override {
            typeFace = 6;
        }
        
        vector<v3f> getVertices() override {
            vector<v3f> vertices;
            return vertices;
        }
        
        vector<unsigned int> getIndices() override {
            vector<unsigned int> indices;
            return indices;
        }
    };
    
    class Circle : public Mesh {
    private:
        float radius = 1.f;
        v3f center;
        int segments = 32;
        
    public:
        void getMesh() override {
            typeFace = 7;
        }
        
        vector<v3f> getVertices() override {
            vector<v3f> vertices;
            return vertices;
        }
        
        vector<unsigned int> getIndices() override {
            vector<unsigned int> indices;
            return indices;
        }
    };
    
    class Dot : public Mesh {
    private:
        v3f position;
        float size = 1.f;
        
    public:
        void getMesh() override {
            typeFace = 8;
        }
        
        vector<v3f> getVertices() override {
            vector<v3f> vertices;
            vertices.push_back(position);
            return vertices;
        }
        
        vector<unsigned int> getIndices() override {
            return vector<unsigned int>();
        }
    };
    
    class Line : public Mesh {
    private:
        v3f start;
        v3f end;
        
    public:
        void getMesh() override {
            typeFace = 9;
        }
        
        vector<v3f> getVertices() override {
            vector<v3f> vertices;
            vertices.push_back(start);
            vertices.push_back(end);
            return vertices;
        }
        
        vector<unsigned int> getIndices() override {
            vector<unsigned int> indices;
            indices.push_back(0);
            indices.push_back(1);
            return indices;
        }
    };
    
    class Polyline : public Mesh {
    private:
        vector<v3f> points;
        
    public:
        void getMesh() override {
            typeFace = 10;
        }
        
        vector<v3f> getVertices() override {
            return points;
        }
        
        vector<unsigned int> getIndices() override {
            vector<unsigned int> indices;
            return indices;
        }
    };
    
    class Dimension : public Mesh {
    private:
        v3f start;
        v3f end;
        float offset = 0.5f;
        
    public:
        void getMesh() override {
            typeFace = 11;
        }
        
        vector<v3f> getVertices() override {
            vector<v3f> vertices;
            return vertices;
        }
        
        vector<unsigned int> getIndices() override {
            vector<unsigned int> indices;
            return indices;
        }
    };
    
    class Bezier : public Mesh {
    private:
        vector<v3f> controlPoints;
        int segments = 32;
        
    public:
        void getMesh() override {
            typeFace = 12;
        }
        
        vector<v3f> getVertices() override {
            vector<v3f> vertices;
            return vertices;
        }
        
        vector<unsigned int> getIndices() override {
            vector<unsigned int> indices;
            return indices;
        }
    };
    
    class Object {
    public:
        string name = "Object0";
        shared_ptr<Mesh> mesh = nullptr;
        Transform transform;
        shared_ptr<Texture> texture = nullptr;

        Object() = default;
        Object(const Object&) = default;
        Object(Object&&) = default;

        Object& operator=(const Object&) = default;
    
        // Operador de atribuição de move
        Object& operator=(Object&&) = default;
        
        // Destrutor
        ~Object() = default;
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
        vector<Object> objects;
        vector<Light> lights;
    
        void add(Object obj)
        {
            objects.push_back(std::move(obj));
        }
    
        void add(Light light)
        {
            lights.push_back(std::move(light));
        }
    
        Object &getObject(string name)
        {
            for (auto &obj : objects)
            {
                if (obj.name == name)
                    return obj;
            }
            static Object invalidObject;
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
        
        void deleteObject(string name)
        {
            for (size_t i = 0; i < objects.size(); i++)
            {
                if (objects[i].name == name)
                {
                    objects.erase(objects.begin() + i);
                    return;
                }
            }
            cout << "err Object " << name << " not found for deletion\n";
        }
        
        void deleteLight(string name)
        {
            for (size_t i = 0; i < lights.size(); i++)
            {
                if (lights[i].name == name)
                {
                    lights.erase(lights.begin() + i);
                    return;
                }
            }
            cout << "err Light " << name << " not found for deletion\n";
        }
    
        void clearAll()
        {
            objects.clear();
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


}