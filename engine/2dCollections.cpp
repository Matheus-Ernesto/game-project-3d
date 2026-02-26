#include <string>
#include <vector>
#include <iostream>

using namespace std;

struct v2f {
    float x = 0.f;
    float y = 0.f;
};

struct v3f {
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
};

struct v4f {
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
    float w = 0.f;
};

class Mesh2d {
public:
    v2f position = {0.f, 0.f};
    int align = 0;
};

class Text : public Mesh2d {
public:
    string text = "Sample";
    int fontSize = 24;
    string pathFont = "fonts/arial.ttf";
};

class Image : public Mesh2d {
public:
    v2f coordinatesStart = {0.f, 0.f};
    v2f coordinatesEnd = {1024.f, 1024.f};
    int stretchType = 0;
    string path = "assets/check_uv_map.png";
    float width = 100.f;
    float height = 100.f;
    float transparency = 255.f;
};

class Circle : public Mesh2d {
public:
    float radius = 50.f;
};

class Line : public Mesh2d {
public:
    v2f coordinatesStart = {0.f, 0.f};
    v2f coordinatesEnd = {100.f, 100.f};
    int fillType = 0;
    float size = 1.f;
};

class Dot : public Mesh2d {
public:
    float size = 5.f;
};

class PolyLine : public Mesh2d {
public:
    vector<v2f> coordinates = {};
    int fillType = 0;
    float size = 1.f;
};

class Rectangle : public Mesh2d {
public:
    float width = 100.f;
    float height = 100.f;
};

class Transform2d {
public:
    v3f position = {0.f, 0.f, 0.f};
    v3f rotation = {0.f, 0.f, 0.f};
    v3f scale = {1.f, 1.f, 1.f};
    v3f deltaVel = {0.f, 0.f, 0.f};
    int align = 0;
};

class Texture2d {
public:
    v4f color = {1.f, 1.f, 1.f, 1.f};
    string path = "";
};

class Object2d {
public:
    string name = "object2d";
    Mesh2d* mesh = nullptr;
    Transform2d transform;
    Texture2d texture;
};

class Gui {
public:
    vector<Object2d> objects;
    
    void add(Object2d obj) {
        objects.push_back(obj);
    }
    
    Object2d& get(string name) {
        for (auto &obj : objects) {  
            if (obj.name == name) {
                return obj;
            }  
        }  
        
        static Object2d invalidObject;  
        cout << "err Obj " << name << " not found\n";  
        return invalidObject;  
    }
    
    void delete_(string name) {  // 'delete' é palavra reservada, use delete_
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