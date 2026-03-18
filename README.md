# game-project-3d: Metal Himiko
In development. Just a 3d game for desktop. Linux and Windows compatible. There is a small 3d engine too.

It uses OpenGL and SFML, with OOP for easy use.

## Stats

Time spent: 52 hours.

## Requirements

A computer with display, just this.

~~The computer I using is a Intel duo E5300 without gpu... So this game runs in a potato with esp32 and 4MB of ram.~~

I finally bought a notebook, is not tha best but is good!!!

## Description

You are a discarded robot, trying to be the best project of your master, so you need to improve yourself, defend your master and destroy others projects with virus
- Top view 3d Game.
- Decisions and 2 End games.

## Goals

Game time: ~1 hour to beat.

Requirements: Run in any potato powered by a Esp32 with 4 MB ram, also runs on Windows and Linux.

Feel free to use any part of this project, just credit the names of the involved with this project.


## Plans:

Engine:
1. Vulkan API or OpenGL API. - OK
2. Create a windowed black screen in OOP format. - OK
3. Get stats, like FPS, RAM usage, Computer stats. - OK 1/3 
4. Clean Coding. - OK
5. Draw shapes. - OK
6. Draw Texts. - OK
7. Draw Images. - OK
8. Draw 3d models. - ON GOING
9. Audio uses. - ON GOING
10. Clean Coding.
11. Importable library.
12. Final documentation.

Not in order:
- FBX Support.
- OBJ Support.
- DWG Support.
- Objects3d:
   - Arc,
   - Cylinder,
   - Sphere with custom resolution
   - Text
   - Triangle
   - Plane
   - Circle
   - Dot
   - Line
   - Poliline
   - Dimension
   - Bezier
- Mesh2d
- Transform
- Modifier
- Song
- Mesh3d
- Reorganize classes


## How to compile:


### Linux

Go to root project

> g++ src/main.cpp -o bin/programa \
    -I./include \
    -L./lib \
    -lsfml-graphics -lsfml-window -lsfml-system -lGL

Or just runs  compile/linux_compile.sh