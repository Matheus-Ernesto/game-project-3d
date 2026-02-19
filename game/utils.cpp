#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <string>

using namespace std;

void fadeLeftOnePitch(vector<string> objects, Engine &engine){
    for (auto obj : objects)
    {
        engine.canvas.gui.getText(obj).x += 0.001f;
        engine.canvas.gui.getText(obj).r += 0.01f;
        engine.canvas.gui.getText(obj).g += 0.01f;
        engine.canvas.gui.getText(obj).b += 0.01f;
    }
};

void changeSelectedOption(vector<string> options, int option, Engine &engine){
    for (auto obj : options)
    {
        engine.canvas.gui.getText(obj).setColor(1.f,1.f,1.f);
    }
    engine.canvas.gui.getText(options[option]).setColor(0.3f,0.3f,1.f);
    
};
struct Vec2i {
    int x, y;

    Vec2i(int x = 0, int y = 0) : x(x), y(y) {}

    Vec2i operator-(const Vec2i& other) const {
        return Vec2i(x - other.x, y - other.y);
    }

    Vec2i operator+(const Vec2i& other) const {
        return Vec2i(x + other.x, y + other.y);
    }

    Vec2i operator*(const Vec2i& other) const {
        return Vec2i(x * other.x, y * other.y);
    }

    void print() const {
        std::cout << "x: " << x << ", y: " << y << std::endl;
    }
};

struct Vec2f{
    float x;
    float y;
    Vec2f(float x = 0.f,float y = 0.f) : x(x), y(y) {}
};