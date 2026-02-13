#include "Scene_Menu.hpp"
#include <iostream>

void Scene_Menu::init()
{
    // Register actions
    // registerAction(sf::Keyboard::P, "PAUSE");

    // m_menuText.setCharacterSize(12);
    // m_menuText.setFont(m_game->assets().getFont("Tech"));
}

void Scene_Menu::update()
{
    sRender();
}

Scene_Menu::Scene_Menu(GameEngine *gameEngine)
    : Scene(gameEngine)
{
    init();
}

void Scene_Menu::sRender()
{
    // sf::Event event;
    // while (m_game->window().pollEvent(event))
    // {
    //     if (event.type == sf::Event::Closed)
    //         m_game->window().close();
    // }
    m_game->window().clear();
    m_game->window().display();
}
