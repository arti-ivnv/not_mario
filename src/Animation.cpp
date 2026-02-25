#include "Animation.hpp"
#include <cmath>
#include <iostream>

Animation::Animation(const std::string &name, const sf::Texture &t, size_t frameCount, size_t speed)
    : m_name(name)
    , m_sprite(t)
    , m_frameCount(frameCount)
    , m_currentFrame(0)
    , m_speed(speed)
{
    m_size = Vec2((float)t.getSize().x / frameCount, (float)t.getSize().y);
    m_sprite.setOrigin(m_size.x / 2.0f, m_size.y / 2.0f);
    m_sprite.setTextureRect(sf::IntRect(std::floor(m_currentFrame) * m_size.x, 0, m_size.x, m_size.y));
    m_sprite.setTexture(t);
}

Animation::Animation(const std::string &name, const sf::Texture &t)
    : Animation(name, t, 1, 0)
{
}

// updates the animation to show the next frame, depending on its speed
// animation loops when it reaches the end
void Animation::update()
{
    // add the speed variable to the current frame
    m_currentFrame++;

    size_t animationFrame = (m_currentFrame / m_speed) % m_frameCount;
    size_t fs             = m_currentFrame / m_speed;

    if (m_name == "MysteryBlock")
    {
        // std::cout << "animationFrame: " << animationFrame << std::endl;
        // std::cout << "fs: " << fs << std::endl;
        if (fs % 20 == 0)
        {
            sf::Color c = m_sprite.getColor();

            int r = c.r, g = c.g, b = c.b;

            if (r == 255 && g == 255 && b == 255)
            {
                r = 225;
                g = 195;
                b = 225;
                m_sprite.setColor(sf::Color(r, g, b));
            }
            else
                m_sprite.setColor(sf::Color(255, 255, 255));
        }
    }

    m_sprite.setTextureRect(sf::IntRect(std::floor(animationFrame) * m_size.x, 0, m_size.x, m_size.y));

    // TODO: 1) calculate the correct frame of animation to play based on currentFrame and speed
    // TODO: 2) set the texture rectangle properly (see constructor for sample)
}

bool Animation::hasEnded() const
{
    // TODO: detect when animation has ended (last frame was played) and return true
    return m_currentFrame == m_frameCount;
}

const std::string &Animation::getName() const
{
    return m_name;
}

const Vec2 &Animation::getSize() const
{
    return m_size;
}

sf::Sprite &Animation::getSprite()
{
    return m_sprite;
}
