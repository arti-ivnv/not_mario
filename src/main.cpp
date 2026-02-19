#include <fstream>
#include <iostream>

#include "GameEngine.hpp"

int main(void)
{
    GameEngine g("data/assets.txt");
    g.run();
    return 0;
}