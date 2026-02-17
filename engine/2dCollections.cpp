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
    string text = "Sample";
    int fontSize = 24;
    string pathFont = "fonts/arial.ttf";

    void setPosition(float x, float y)
    {
        x = x;
        y = y;
    }

    void setColor(float r, float g, float c)
    {
        r = r;
        g = g;
        b = b;
    }
};

class Menu
{
    float x = 0.f, y = 0.f;
    float r = 0.f, g = 0.f, b = 0.f;
    int align = 0;
    vector<Text> options;
    Text selectedTextFormat;
};

class Image
{
public:
    float x = 0.f, y = 0.f;
    int align;
    int x1;
    int y1;
    int x2;
    int y2;
    int width;
    int height;
    float transparency;
    string path;
};

class Circle
{
public:
    float x = 0.f, y = 0.f;
    float r = 0.f, g = 0.f, b = 0.f;
    int align = 0;
    float radius = 0;

    void setPosition(float x, float y)
    {
        x = x;
        y = y;
    }

    void setColor(float r, float g, float c)
    {
        r = r;
        g = g;
        b = b;
    }
};