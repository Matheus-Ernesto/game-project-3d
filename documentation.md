# Documentation

Documentation about the engine, how to use and what it is.

The Engine is a library (almost a framework) that uses SFML and OpenGL to make programs with 2d and 3d. It is porcetage based for 2d and Coordinates for 3d.

- SFML - A library for 2d.
- OpenGL - API for 3d, 2d and others.

## How to use:

Firstly, you create a object Engine, so you start and make a loop, this loop will be for renders, also, you make a inside loop for game logic (recommended).

There is a huge example of a game (dev. In progress).

There is a OOP Docs:


### Configs


Variables

| type   | Name           | Default Value |
|--------|----------------|---------------|
| int    | widthWindow    | 800           |
| int    | heightWindow   | 600           |
| int    | audioVolume    | 10            |
| bool   | fullscreen     | false         |
| bool   | showFPS        | true          |


Functions

*None*

---

### Loader

Variables

*None*

Functions

| Return Type | Name with Params        | Usage                                                     |
|-------------|-------------------------|-----------------------------------------------------------|
| int         | saveConfig(Configs config) | Saves the configuration to a file. Returns `0` if successful, or `1` if an error occurs. |
| Configs     | loadConfig()            | Loads the configuration from a file and returns the `Configs` structure. If an error occurs, returns a default `Configs` object. |


---

### Canvas

Variables

| type         | Name                               | Default Value |
|--------------|------------------------------------|---------------|
| inline static| totalFrames                        | 0             |
| inline static| last                               | chrono::high_resolution_clock::now() |
| inline static| fps                                | 0             |
| inline static| font                               | sf::Font()    |
| vector       | texts                              | Empty vector  |
| vector       | circles                            | Empty vector  |
| vector       | sprites                            | Empty vector  |
| vector       | textures                           | Empty vector  |
| constexpr    | ALIGN_CENTER                       | 0             |
| struct       | Color                              | float r, g, b |

Functions

| Return   | Name                        | Usage                                                                 |
|---------------|---------------------------------------|-----------------------------------------------------------------------|
| void          | draw(sf::RenderWindow &window)        | Draws all the text, circle, and sprite elements on the window.        |
| void          | apply(sf::RenderWindow &window, int width, int height, bool showFps) | Resets GL states, shows FPS if enabled, and calls `draw`.              |
| void          | text(float xpercetage, float ypercetage, string text, int size, int position, float r, float g, float b) | Adds a text element to the `texts` vector at the specified position and with given color. |
| void          | circle(float xpercetage, float ypercetage, float radius, int align, float r, float g, float b) | Adds a circle element to the `circles` vector at the specified position with given color. |
| void          | image(float xpercetage, float ypercetage, string path, int align, int x1, int y1, int x2, int y2, int width, int height, float transparency) | Loads and adds an image sprite to the `sprites` vector with scaling and transparency. |
| void          | clearAll()                            | Clears all the elements from `texts`, `circles`, `sprites`, and `textures` vectors. |

---

### Camera

Variables

| type   | Name           | Default Value |
|--------|----------------|---------------|
| float  | rotationX      | None          |
| float  | rotationY      | None          |
| float  | rotationZ      | None          |
| float  | x              | None          |
| float  | y              | None          |
| float  | z              | None          |
| float  | fov            | None          |
| float  | limitNear      | None          |
| float  | limitFar       | None          |

Functions

*None*

---

### Scene

Variables

*None*

Functions

*None*

---

### World

Variables

| type   | Name                       | Default Value |
|--------|----------------------------|---------------|
| float  | colorBackgroud_r           | 0.f           |
| float  | colorBackgroud_g           | 0.f           |
| float  | colorBackgroud_b           | 0.f           |
| Camera | camera                     | Default Camera values |

Functions

| Return Type | Name with Params      | Usage                                                 |
|-------------|-----------------------|-------------------------------------------------------|
| void        | setBackgroundColor(float r, float g, float b) | Sets the background color of the scene.               |
| void        | drawBackground()       | Draws the background with the set color.             |
| void        | draw()                 | Draws a 3D cube with various colored faces.           |

---

### Canvas3D

Variables

| type   | Name           | Default Value |
|--------|----------------|---------------|
| World  | world          | Default World values |
| bool   | enable3D       | None          |

Functions

| Return Type | Name with Params              | Usage                                                     |
|-------------|-------------------------------|-----------------------------------------------------------|
| void        | draw(sf::RenderWindow & window) | Initializes OpenGL settings for 3D rendering and draws the scene. |
| void        | apply(sf::RenderWindow & window, int width, int height, bool showFps) | Calls `draw()` to render the scene.                        |

---

### Engine

Variables

| type             | Name                    | Default Value                      |
|------------------|-------------------------|------------------------------------|
| unordered_set    | keysDown                | Empty unordered_set                |
| unordered_set    | mouseDown               | Empty unordered_set                |
| sf::Vector2i     | mousePosition           | {0, 0}                             |
| float            | mouseScrollDelta        | 0.f                                |
| inline static    | timeGameStep            | 16                                 |
| inline static    | fullscreen              | false                              |
| inline static    | width                   | 100                                |
| inline static    | height                  | 100                                |
| inline static    | showFPS                 | false                              |
| inline static    | windowName              | "Test"                             |
| inline static    | quitForce               | false                              |
| inline static    | canvas                  | Default Canvas                     |
| inline static    | canvas3d                | Default Canvas3D                   |
| inline static    | window                  | Default sf::RenderWindow           |

Functions

| Return Type | Name with Params                        | Usage                                                         |
|-------------|-----------------------------------------|---------------------------------------------------------------|
| void        | reshape(int t_width, int t_height)     | Updates the window size when the window is resized.           |
| void        | display()                               | Clears the window, applies the 2D and 3D canvas, and displays. |
| void        | start()                                 | Initializes the window, sets the resolution, and loads the font. |
| void        | pollEvents()                            | Polls for events, updates input states (keys, mouse, scroll). |
| bool        | getKeyDown(sf::Keyboard::Key key)       | Checks if a key was pressed.                                  |
| bool        | getKey(sf::Keyboard::Key key)           | Checks if a key is currently pressed.                         |
| bool        | getMouseKey(sf::Mouse::Button button)   | Checks if a mouse button was pressed.                         |
| sf::Vector2i| getMouseMove()                          | Returns the current mouse position.                           |
| float       | getMouseScroll()                        | Returns the total mouse scroll delta.                         |
| bool        | quit()                                  | Checks if the window should be closed.                        |
| void        | exit()                                  | Closes the window.                                            |

---