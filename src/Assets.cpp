#include "Assets.hpp"
#include <iostream>

void Assets::loadFromFile(const std::string &fileName)
{
    std::cout << "==========  assets.txt started to proceed ==========\n";
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
        else if (fpoint == "Animation")
        {
            std::string name;
            std::string texName;
            int         frameCount;
            int         animSpeed;

            fin >> name >> texName >> frameCount >> animSpeed;

            Animation animation(name, getTexture(texName), frameCount, animSpeed);
            addAnimation(name, animation);
        }
    }

    std::cout << "==========  assets.txt proceeded ==========\n";
}

const sf::Texture &Assets::getTexture(const std::string &name) const
{
    return m_textures.at(name);
}

const Animation &Assets::getAnimation(const std::string &name) const
{
    return m_animations.at(name);
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

    m_fonts[name] = temp;
    std::cout << "Font " << name << " loaded. path: " << path << '\n';
}

void Assets::addTexture(const std::string &name, const std::string &path)
{
    sf::Texture temp;

    if (!temp.loadFromFile(path, sf::IntRect({32, 32}, {0, 0})))
    {
        std::cerr << "Failed to load texture: " << name << " with a path " << path << '\n';
    }

    m_textures[name] = temp;
    std::cout << "Texture " << name << " loaded. path: " << path << '\n';
}

void Assets::addAnimation(const std::string &name, const Animation &animation)
{
    m_animations[name] = animation;
    std::cout << "Animation " << name << " loaded. \n";
}
