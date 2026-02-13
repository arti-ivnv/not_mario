#pragma once

#include "Animation.hpp"

class Assets
{
    std::map<std::string, sf::Texture> m_textures;
    std::map<std::string, Animation>   m_animations;
    std::map<std::string, sf::Font>    m_fonts;

  public:
    Assets() = default;
};