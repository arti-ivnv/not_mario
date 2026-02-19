#pragma once

#include "Animation.hpp"
#include <fstream>

class Assets
{
    std::map<std::string, sf::Texture> m_textures;
    std::map<std::string, Animation>   m_animations;
    std::map<std::string, sf::Font>    m_fonts;

    void addTexture(const std::string &name, const std::string &path);
    void addAnimation(const std::string &name, const Animation &animation);
    // void addSound(const std::string &name, const std::string &path);
    void addFont(const std::string &name, const std::string &path);

  public:
    Assets() = default;

    void loadFromFile(const std::string &fileName);

    const sf::Texture &getTexture(const std::string &name) const;
    const Animation   &getAnimation(const std::string &name) const;
    const sf::Font    &getFont(const std::string &name) const;
};