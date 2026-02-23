# game-project-3d: Metal Himiko
In development. Just a 3d game for desktop. Linux and Windows compatible. There is a small 3d engine too.

## Stats

Time spent: 50 hours.

## Requirements

A computer with display, just this.

The computer I using is a Intel duo E5300 without gpu... So this game runs in a potato with esp32 and 4MB of ram.


## Description

You are a discarded robot, trying to be the best project of your master, so you need to improve yourself, defend your master and destroy others projects with virus
- Top view 3d Game.
- Decisions and 2 End games.

## Goals

Game time: ~1 hour to beat.

Requirements: Run in any potato powered by a Esp32 with 4 MB ram, also runs on Windows and Linux.

Feel free to use any part of this project, just credit the names of the involved with this project.


People making project:


Matheus-Ernesto (github) - Code Project - Developer.

hidden (telegram) - Concept Art - Artist

hidden (X - old Twitter) - Concept Art - Artist

hidden (whatsapp) - Concept Art - Artist


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
- Objects3d: Arc, Cylinder, Sphere with custom resolution, text, triangle, plane, circle, dot, line, poliline, dimension, bezier.
- Typefaces.
- Classes: mesh(3d/2d), transform, modifiers...

Engine
audio
2d
3d
window
configs
- start()
- exit()
- syncConfigsToWindow()
bool isWindowClosing()

Audio
vector songs
vector sounds
- playSong(sound)
- pauseSong(name)
- resumeSong(name)
- removeSong(name)
- isPlaying(name)
- playSound(path, vel, volume)

Song
name
path
vel
volume
- constructor()

2d
vector gui
background
foreground
vector effects

3d
vector scene
background
foreground
vector effects


Gui
- add()
- remove()
- removeAll()

scene
- add()
- remove()
- removeAll()


Object2D
mesh2d
transform
vector effect
vector modifiers

Object3D
mesh3d
transform
texture
vector effect
vector modifiers

Mesh3d
type 
0 - cube
1 - triangle
2 - sphere
3 - plane
4 - circle
5 - dot
6 - line
7 - poliline
8 - model
9 - arc
10 - text
11 - dimension
12 - cylinder
13 - bezier

visible
calculable
typeFace Triangles/Quads/NGon
vector faces
- isHit();
- distanceTo();
v3f getExtreme(v3f direction);


Cube implements Mesh3d

Triangle implements Mesh3d
point

Sphere implements Mesh3d
resolution

Plane implements Mesh3d

Circle implements Mesh3d

Dot implements Mesh3d
x1 x2 x3 y1 y2 y3

Line implements Mesh3d

Poliline implements Mesh3d

Arc implements Mesh3d

Text implements Mesh3d

Model implements Mesh3d
path
format
- import(path, format)

Transform
v3f position
v3f scale
v3f rotation
v3f deltaVel

Camera
transform
v3f look
fov
limitNear
limitFar