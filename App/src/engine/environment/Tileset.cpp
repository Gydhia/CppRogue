#include "Tileset.hpp"

namespace cppRogue::environment {

sf::IntRect Tileset::rect(int globalTileId) const
{
    ASSERT_DEBUG(globalTileId >= firstGid && globalTileId < tileCount + firstGid,
                 "globalTileId cannot be lower than firstGid");

    int tileId = globalTileId - firstGid;

    auto left = (tileId % nbColumns) * (tileWidth + spacing) + margin;
    auto top = (tileId / nbColumns) * (tileHeight + spacing) + margin;
    // Left --> Top --> Width --> Height
    return sf::IntRect(left, top, tileWidth, tileHeight);
}

Tileset::Tileset(int numberOftiles, const sf::Texture& tilesetTexture)
    : tileCount(numberOftiles), texture(tilesetTexture), m_infoIds(numberOftiles, 0){};

} // namespace cppRogue::environment
