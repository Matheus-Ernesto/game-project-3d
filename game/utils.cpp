#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <string>

#include <cmath>

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

#include <iostream>
#include <cmath>

// Vec2i - Vector 2D de inteiros
struct Vec2i {
    int x, y;

    Vec2i(int x = 0, int y = 0) : x(x), y(y) {}

    // Operadores aritméticos
    Vec2i operator-() const { return Vec2i(-x, -y); }
    
    Vec2i operator-(const Vec2i& other) const {
        return Vec2i(x - other.x, y - other.y);
    }

    Vec2i operator+(const Vec2i& other) const {
        return Vec2i(x + other.x, y + other.y);
    }

    Vec2i operator*(const Vec2i& other) const {
        return Vec2i(x * other.x, y * other.y);
    }
    
    Vec2i operator*(int scalar) const {
        return Vec2i(x * scalar, y * scalar);
    }
    
    Vec2i operator/(int scalar) const {
        return Vec2i(x / scalar, y / scalar);
    }
    
    // Operadores de atribuição
    Vec2i& operator+=(const Vec2i& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    
    Vec2i& operator-=(const Vec2i& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    
    Vec2i& operator*=(const Vec2i& other) {
        x *= other.x;
        y *= other.y;
        return *this;
    }
    
    Vec2i& operator*=(int scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    
    Vec2i& operator/=(int scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }
    
    // Operadores de comparação
    bool operator==(const Vec2i& other) const {
        return x == other.x && y == other.y;
    }
    
    bool operator!=(const Vec2i& other) const {
        return !(*this == other);
    }
    
    // Funções úteis
    float length() const {
        return std::sqrt(static_cast<float>(x*x + y*y));
    }
    
    int dot(const Vec2i& other) const {
        return x * other.x + y * other.y;
    }
    
    void print() const {
        std::cout << "x: " << x << ", y: " << y << std::endl;
    }
};

// Vec3i - Vector 3D de inteiros
struct Vec3i {
    int x, y, z;

    Vec3i(int x = 0, int y = 0, int z = 0) : x(x), y(y), z(z) {}

    // Operadores aritméticos
    Vec3i operator-() const { return Vec3i(-x, -y, -z); }
    
    Vec3i operator-(const Vec3i& other) const {
        return Vec3i(x - other.x, y - other.y, z - other.z);
    }

    Vec3i operator+(const Vec3i& other) const {
        return Vec3i(x + other.x, y + other.y, z + other.z);
    }

    Vec3i operator*(const Vec3i& other) const {
        return Vec3i(x * other.x, y * other.y, z * other.z);
    }
    
    Vec3i operator*(int scalar) const {
        return Vec3i(x * scalar, y * scalar, z * scalar);
    }
    
    Vec3i operator/(int scalar) const {
        return Vec3i(x / scalar, y / scalar, z / scalar);
    }
    
    // Operadores de atribuição
    Vec3i& operator+=(const Vec3i& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    
    Vec3i& operator-=(const Vec3i& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    
    Vec3i& operator*=(const Vec3i& other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }
    
    Vec3i& operator*=(int scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
    
    Vec3i& operator/=(int scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }
    
    // Operadores de comparação
    bool operator==(const Vec3i& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    
    bool operator!=(const Vec3i& other) const {
        return !(*this == other);
    }
    
    // Funções úteis
    float length() const {
        return std::sqrt(static_cast<float>(x*x + y*y + z*z));
    }
    
    int dot(const Vec3i& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    
    Vec3i cross(const Vec3i& other) const {
        return Vec3i(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }
    
    void print() const {
        std::cout << "x: " << x << ", y: " << y << ", z: " << z << std::endl;
    }
};

// Vec2f - Vector 2D de floats
struct Vec2f {
    float x, y;

    Vec2f(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

    // Operadores aritméticos
    Vec2f operator-() const { return Vec2f(-x, -y); }
    
    Vec2f operator-(const Vec2f& other) const {
        return Vec2f(x - other.x, y - other.y);
    }

    Vec2f operator+(const Vec2f& other) const {
        return Vec2f(x + other.x, y + other.y);
    }

    Vec2f operator*(const Vec2f& other) const {
        return Vec2f(x * other.x, y * other.y);
    }
    
    Vec2f operator*(float scalar) const {
        return Vec2f(x * scalar, y * scalar);
    }
    
    Vec2f operator/(float scalar) const {
        return Vec2f(x / scalar, y / scalar);
    }
    
    // Operadores de atribuição
    Vec2f& operator+=(const Vec2f& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    
    Vec2f& operator-=(const Vec2f& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    
    Vec2f& operator*=(const Vec2f& other) {
        x *= other.x;
        y *= other.y;
        return *this;
    }
    
    Vec2f& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    
    Vec2f& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }
    
    // Operadores de comparação
    bool operator==(const Vec2f& other) const {
        const float epsilon = 1e-6f;
        return std::abs(x - other.x) < epsilon && std::abs(y - other.y) < epsilon;
    }
    
    bool operator!=(const Vec2f& other) const {
        return !(*this == other);
    }
    
    // Funções úteis
    float length() const {
        return std::sqrt(x*x + y*y);
    }
    
    Vec2f normalized() const {
        float len = length();
        if (len > 0) {
            return Vec2f(x / len, y / len);
        }
        return Vec2f(0, 0);
    }
    
    void normalize() {
        float len = length();
        if (len > 0) {
            x /= len;
            y /= len;
        }
    }
    
    float dot(const Vec2f& other) const {
        return x * other.x + y * other.y;
    }
    
    void print() const {
        std::cout << "x: " << x << ", y: " << y << std::endl;
    }
};

// Vec3f - Vector 3D de floats
struct Vec3f {
    float x, y, z;

    Vec3f(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}

    // Operadores aritméticos
    Vec3f operator-() const { return Vec3f(-x, -y, -z); }
    
    Vec3f operator-(const Vec3f& other) const {
        return Vec3f(x - other.x, y - other.y, z - other.z);
    }

    Vec3f operator+(const Vec3f& other) const {
        return Vec3f(x + other.x, y + other.y, z + other.z);
    }

    Vec3f operator*(const Vec3f& other) const {
        return Vec3f(x * other.x, y * other.y, z * other.z);
    }
    
    Vec3f operator*(float scalar) const {
        return Vec3f(x * scalar, y * scalar, z * scalar);
    }
    
    Vec3f operator/(float scalar) const {
        return Vec3f(x / scalar, y / scalar, z / scalar);
    }
    
    // Operadores de atribuição
    Vec3f& operator+=(const Vec3f& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    
    Vec3f& operator-=(const Vec3f& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    
    Vec3f& operator*=(const Vec3f& other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }
    
    Vec3f& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
    
    Vec3f& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }
    
    // Operadores de comparação
    bool operator==(const Vec3f& other) const {
        const float epsilon = 1e-6f;
        return std::abs(x - other.x) < epsilon && 
               std::abs(y - other.y) < epsilon && 
               std::abs(z - other.z) < epsilon;
    }
    
    bool operator!=(const Vec3f& other) const {
        return !(*this == other);
    }
    
    // Funções úteis
    float length() const {
        return std::sqrt(x*x + y*y + z*z);
    }
    
    Vec3f normalized() const {
        float len = length();
        if (len > 0) {
            return Vec3f(x / len, y / len, z / len);
        }
        return Vec3f(0, 0, 0);
    }
    
    void normalize() {
        float len = length();
        if (len > 0) {
            x /= len;
            y /= len;
            z /= len;
        }
    }
    
    float dot(const Vec3f& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    
    Vec3f cross(const Vec3f& other) const {
        return Vec3f(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }
    
    void print() const {
        std::cout << "x: " << x << ", y: " << y << ", z: " << z << std::endl;
    }
};