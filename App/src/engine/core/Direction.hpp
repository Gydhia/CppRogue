#ifndef CPPROGUE_DIRECTION_HPP
#define CPPROGUE_DIRECTION_HPP

// @EXERCICE 2

#include <SFML/System/Vector2.hpp>
#include <array>

namespace cppRogue::direction {

/**
 * @brief      Set of available direction for an entity. An entity can move on cardinal and
 *             intercardinal directions.
 */
enum class Ordinal
{
    None,
    N,
    S,
    E,
    W,
    NE,
    NW,
    SE,
    SW
};

sf::Vector2i numeric(Ordinal ordinalDirection);

} // namespace cppRogue::direction

#endif
