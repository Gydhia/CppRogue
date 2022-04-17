#ifndef CPPROGUE_GRAPHICSINFO_HPP
#define CPPROGUE_GRAPHICSINFO_HPP

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

namespace cppRogue {

/**
 * @brief      Container of minimal required data to display a sprite.
 */
struct GraphicsInfo
{
    explicit GraphicsInfo(const sf::Texture& textureData)
    {
        sprite.setTexture(textureData);
        sprite.setPosition(0, 0);
    }

    inline void setTexture(const sf::Texture& textureData) { sprite.setTexture(textureData); }
    inline void setPosition(sf::Vector2f position) { sprite.setPosition(position); }
    inline void setBounds(const sf::IntRect& bounds) { sprite.setTextureRect(bounds); }

    sf::Sprite sprite{};
};

} // namespace cppRogue

#endif
