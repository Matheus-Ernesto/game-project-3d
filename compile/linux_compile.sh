#!/bin/bash

cd ../game

# Compilar
g++ game.cpp -o test1.o \
    -I../libs/linux/SFML/include \
    -I../libs/linux/glew/include \
    -I../libs/linux/glm \
    -L../libs/linux/SFML/lib \
    -L../libs/linux/glew/lib \
    -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio \
    -lGLEW \
    -lGL -lGLU \
    -std=c++17 \
    -Wl,-rpath,'$ORIGIN/../libs/linux/SFML/lib' \
    -Wl,-rpath,'$ORIGIN/../libs/linux/glew/lib'