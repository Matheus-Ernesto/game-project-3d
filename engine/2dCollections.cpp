#include <SFML/Graphics.hpp>
#include <memory>
#include <chrono>
#include <vector>
#include <string>
#include <iostream>
#include <typeinfo>

namespace Collections2d {
    class Mesh {
    public:
        virtual ~Mesh() = default;  // torna a classe polimórfica
    };

    class Text : public Mesh {
    public:
        string text = "Sample";
        int fontSize = 24;
        string pathFont = "fonts/arial.ttf";
    };

    class Image : public Mesh {
    public:
        v2f coordinatesStart = {0.f, 0.f};
        v2f coordinatesEnd = {1024.f, 1024.f};
        int stretchType = 0;
        string path = "assets/check_uv_map.png";
        float width = 100.f;
        float height = 100.f;
        float transparency = 255.f;
    };

    class Circle : public Mesh {
    public:
        float radius = 50.f;
    };

    // Outros tipos (Line, Dot, etc.) podem ser adicionados posteriormente

    class Transform {
    public:
        v3f position = {0.f, 0.f, 0.f};
        v3f rotation = {0.f, 0.f, 0.f};
        v3f scale = {1.f, 1.f, 1.f};
        v3f deltaVel = {0.f, 0.f, 0.f};
        int align = 0;
    };

    class Texture {
    public:
        v4f color = {1.f, 1.f, 1.f, 1.f}; // x=r, y=g, z=b, w=a
        string path = "";
    };

    class Object {
    public:
        string name = "object";
        Mesh* mesh = nullptr;
        Transform transform;
        Texture texture;
    };

    class Gui {
    public:
        vector<Object> objects;

        void addObj(Object obj) {
            objects.push_back(obj);
        }

        Object& get(string name) {
            for (auto &obj : objects) {
                if (obj.name == name) {
                    return obj;
                }
            }
            static Object invalidObject;
            cout << "err Obj " << name << " not found\n";
            return invalidObject;
        }

        void deleteObj(string name) {
            for (size_t i = 0; i < objects.size(); i++) {
                if (objects[i].name == name) {
                    objects.erase(objects.begin() + i);
                    cout << "Obj " << name << " removed successfully\n";
                    return;
                }
            }
            cout << "err Obj " << name << " not found for deletion\n";
        }

        void deleteAll() {
            objects.clear();
        }
    };
}