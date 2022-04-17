#include "Tile.hpp"

namespace cppRogue::environment {

Tile::Tile(const TileInfo& tileInfo) : m_info(tileInfo){};

bool Tile::canEnter(Motilities entityMotilities) const
{
    // Test if there is any intersection in motilities
    entityMotilities &= m_info.motilityFlags;
    return entityMotilities.any();
}

} // namespace cppRogue::environment
