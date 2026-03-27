// test_glew.cpp
#include <GL/glew.h>
#include <iostream>

int main() {
    std::cout << "GLEW version: " << glewGetString(GLEW_VERSION) << std::endl;
    return 0;
}