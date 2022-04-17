#include "Direction.hpp"

namespace cppRogue::direction {

/* Map from ordinal direction to numeric direction */
const std::array<sf::Vector2i, 9> DirectionToMove = {sf::Vector2i{0, 0},   // none
                                                     sf::Vector2i{0, -1},  // north
                                                     sf::Vector2i{0, 1},   // south
                                                     sf::Vector2i{1, 0},   // east
                                                     sf::Vector2i{-1, 0},  // west
                                                     sf::Vector2i{1, -1},  // east - north
                                                     sf::Vector2i{-1, -1}, // west - north
                                                     sf::Vector2i{1, 1},   // east - south
                                                     sf::Vector2i{-1, 1}}; // west - south

sf::Vector2i numeric(Ordinal ordinalDirection)
{
    const auto index = static_cast<std::size_t>(ordinalDirection);
    return DirectionToMove[index];
}

} // namespace cppRogue::direction
