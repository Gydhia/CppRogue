#ifndef CPPROGUE_TILESET_HPP
#define CPPROGUE_TILESET_HPP

#include "engine/core/utility/Debug.hpp"

#include <SFML/Graphics/Texture.hpp>

#include <string>

namespace cppRogue::environment {

struct Tileset
{
  public:
    Tileset() = delete;
    Tileset(int numberOftiles, const sf::Texture& tilesetTexture);

    [[nodiscard]] sf::IntRect rect(int globalTileId) const;

    /**
     * @brief      Retrieve information about the tile type (not the tile itself).
     *
     * @param[in]  globalTileId  The global tile identifier
     *
     * @return     TileInfo id attached to this the tile id @p globalTileId
     */
    [[nodiscard]] inline int tileInfoId(int globalTileId) const
    {
        const int tileId = globalTileId - firstGid;
        return m_infoIds[tileId];
    }

    void setInfoId(int localId, int infoId) { m_infoIds[localId] = infoId; }

    std::string name{};
    int firstGid{};
    int imageHeight{};
    int imageWidth{};

    int nbColumns{};
    int tileHeight{};
    int tileWidth{};
    // Space between image edge and first tile (pixels)
    int margin{};
    // Space between adjacent tiles in image (pixels)
    int spacing{};

    const int tileCount;
    sf::Texture texture;

  private:
    std::vector<int> m_infoIds;
};

} // namespace cppRogue::environment

#endif
