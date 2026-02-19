#include "Assets.hpp"
#include <iostream>

void Assets::loadFromFile(const std::string &fileName)
{

    std::fstream fin(fileName);
    std::string  fpoint;

    // std::cout << fin.is_open() << '\n';
    while (fin >> fpoint)
    {
        if (fpoint == "Font")
        {
            std::string name;
            std::string path;

            fin >> name >> path;

            addFont(name, path);
        }
        else if (fpoint == "Texture")
        {
            std::string name;
            std::string path;

            fin >> name >> path;

            addTexture(name, path);
        }
    }

    std::cout << "assets.txt proceeded \n";
}

const sf::Texture &Assets::getTexture(const std::string &name) const
{
    return m_textures.at(name);
}

const sf::Font &Assets::getFont(const std::string &name) const
{
    return m_fonts.at(name);
}

void Assets::addFont(const std::string &name, const std::string &path)
{
    sf::Font temp;

    if (!temp.loadFromFile(path))
    {
        std::cerr << "Failed to load font: " << name << " with a path " << path << '\n';
    }

    std::cout << "Font " << name << " loaded. path: " << path << '\n';
    m_fonts[name] = temp;
}

void Assets::addTexture(const std::string &name, const std::string &path)
{
    sf::Texture temp;

    if (!temp.loadFromFile(path, sf::IntRect({32, 32}, {0, 0})))
    {
        std::cerr << "Failed to load texture: " << name << " with a path " << path << '\n';
    }

    std::cout << "Texture " << name << " loaded. path: " << path << '\n';
    m_textures[name] = temp;
}