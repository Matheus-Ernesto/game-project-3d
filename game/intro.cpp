#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace std;

int intro(Engine &engine)
{  
    int color = 0;

    while (!engine.quit())
    {
        engine.pollEvents();
        

        engine.display();
    }
    return 0;
};
