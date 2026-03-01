#include <fstream>
#include <iostream>

#include "GameEngine.hpp"
#include "Resource_Path.hpp"

int main(void)
{

    GameEngine g(getResourcePath() + "data/assets.txt");
    g.run();
    return 0;
}