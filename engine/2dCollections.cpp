#include <SFML/Graphics.hpp>
#include <memory>
#include <chrono>
#include <string>

using namespace std;

class Text
{
public:
    float x = 0.f, y = 0.f;
    float r = 1.f, g = 1.f, b = 1.f;
    int align = 0;
    string name = "";
    string text = "Sample";
    int fontSize = 24;
    string pathFont = "fonts/arial.ttf";

    void setPosition(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

    void setColor(float r, float g, float b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }
};

class Image
{
public:
    float x = 0.5f; // Meio da tela na horizontal
    float y = 0.5f; // Meio da tela na vertical
    int align = 1;  // ALIGN_CENTER (assumindo que 1 = centro)
    string name = "check_uv_map";
    int x1 = 0;               // Começo da textura no x
    int y1 = 0;               // Começo da textura no y
    int x2 = 1024;            // Largura da textura
    int y2 = 1024;            // Altura da textura
    float width = 100.f;         // Largura na tela
    float height = 100.f;        // Altura na tela
    float transparency = 255; // Totalmente opaco
    string path = "assets/check_uv_map.png";
};

class Circle
{
public:
    float x = 0.f, y = 0.f;
    float r = 0.f, g = 0.f, b = 0.f;
    int align = 0;
    float radius = 0;
    string name = "";

    void setPosition(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

    void setColor(float r, float g, float b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }
};

class GUI
{
public:
    vector<Text> texts;
    vector<Circle> circles;
    vector<Image> images;

    void add(Text text)
    {
        texts.push_back(text);
    }
    void add(Circle circle)
    {
        circles.push_back(circle);
    }
    void add(Image image)
    {
        images.push_back(image);
    }

    Text &getText(string name)
    {
        for (auto &obj : texts)
        {
            if (obj.name == name)
                return obj;
        }
        static Text invalidObject;
        cout << "err Obj " << name << " not found\n";
        return invalidObject;
    }

    Circle &getCircle(string name)
    {
        for (auto &obj : circles)
        {
            if (obj.name == name)
                return obj;
        }
        static Circle invalidObject;
        cout << "err Obj " << name << " not found\n";
        return invalidObject;
    }

    Image &getImage(string name)
    {
        for (auto &obj : images)
        {
            if (obj.name == name)
                return obj;
        }
        static Image invalidObject;
        cout << "err Obj " << name << " not found\n";
        return invalidObject;
    }

    void clearAll()
    {
        texts.clear();
        circles.clear();
        images.clear();
    }
};