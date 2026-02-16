#include "Scene_Menu.hpp"

#include <iostream>

void Scene_Menu::init()
{
    // Register actions
    registerAction(sf::Keyboard::W, "UP");
    registerAction(sf::Keyboard::S, "DOWN");

    m_title = "Not Mega Mario";

    m_menuStrings.push_back("LEVEL 1");
    m_menuStrings.push_back("LEVEL 2");
    m_menuStrings.push_back("LEVEL 3");

    m_menuStrings.push_back("UP : W");
    m_menuStrings.push_back("DOWN : S");
    m_menuStrings.push_back("PLAY : D");
    m_menuStrings.push_back("BACK : ESC");

    if (!m_font.loadFromFile("assets/fonts/ByteBounce.ttf"))
    {
        std::cerr << "Failed to load font :( \n";
        return;
    }

    m_menuText = sf::Text(m_title, m_font);
    m_menuText.setCharacterSize(100); // 12
    m_menuText.setFillColor(sf::Color::Black);
    m_menuText.setPosition(sf::Vector2f(40, 0));
}

void Scene_Menu::update()
{
    sRender();
}

void Scene_Menu::sDoAction(const Action &action)
{
    if (action.type() == "START")
    {
        if (action.name() == "DOWN")
        {
            if (m_selectedMenuIndex != 2) // TODO: Change to m_levelPaths.size() - 1
                m_selectedMenuIndex++;
        }

        if (action.name() == "UP")
        {
            if (m_selectedMenuIndex != 0)
                m_selectedMenuIndex--;
        }
    }
}

Scene_Menu::Scene_Menu(GameEngine *gameEngine)
    : Scene(gameEngine)
{
    init();
}

void Scene_Menu::sRender()
{
    m_game->window().clear(sf::Color(111, 111, 246, 255));

    // DRAW TITLE
    m_menuText.setString(m_title);
    m_game->window().draw(m_menuText);

    // DRAW LEVELS
    for (size_t i = 1; i <= m_menuStrings.size() - 4; i++)
    {
        sf::Text temp(m_menuStrings[i - 1], m_font, 100);
        temp.setPosition(sf::Vector2f(40, (80 * i) + 40));
        if (m_selectedMenuIndex == i - 1)
        {
            temp.setColor(sf::Color::White);
        }
        else
        {
            temp.setColor(sf::Color::Black);
        }

        m_game->window().draw(temp);
    }

    // DRAW BUTTON INSTRUCTIONS
    for (size_t i = m_menuStrings.size() - 3, j = 0; i <= m_menuStrings.size(); i++, j++)
    {
        sf::Text temp(m_menuStrings[i - 1], m_font, 60);
        temp.setPosition(sf::Vector2f((250 * j) + 40, m_game->window().getSize().y - 100));
        temp.setColor(sf::Color::Black);

        m_game->window().draw(temp);
    }

    m_game->window().display();
}
