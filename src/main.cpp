#include <fstream>
#include <iostream>

#include "GameEngine.hpp"

int main(void)
{
    GameEngine g("config.txt");
    g.run();
    return 0;
}