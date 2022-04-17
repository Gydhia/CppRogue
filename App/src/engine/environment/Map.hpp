#ifndef CPPROGUE_MAP_HPP
#define CPPROGUE_MAP_HPP

#include "Tile.hpp"
#include "Tileset.hpp"
#include "engine/core/GraphicsInfo.hpp"

#include "nlohmann/json.hpp"

#include <SFML/System/Vector2.hpp>

#include <cstdint>
#include <string>
#include <vector>

namespace cppRogue::environment {

/**
 * @brief      Container to make a clear distinction between world position and map
 *             coordinates.
 *               - World position use sf::Vector2i
 *               - Map position use MapCoords.
 */
struct MapCoords
{
    std::int32_t x{};
    std::int32_t y{};
};

/**
 * @brief      A collection of Tile of different kinds. A map is can have one or multiple
 *             TileSet associated.
 */
class Map : public sf::Drawable
{
  public:
    struct Dimensions
    {
      public:
        std::int32_t tileWidth;
        std::int32_t tileHeight;
        std::int32_t rowCount;
        std::int32_t columCount;
    };

    Map() = default;
    ~Map() override = default;

    [[nodiscard]] inline std::int32_t width() const { return m_dimension.columCount; }
    [[nodiscard]] inline std::int32_t height() const { return m_dimension.rowCount; }

    /**
     * @brief      Convert from world position to map coordinates.
     *
     * @param[in]  position  The world position in pixel.
     *
     * @return     Corresponding coordinates inside the map for @p position.
     */
    [[nodiscard]] MapCoords coordinates(const sf::Vector2i& position) const;

    [[nodiscard]] inline Tile& at(const MapCoords& coords)
    {
        return const_cast<Tile&>(static_cast<const Map&>(*this).at(coords));
    }

    [[nodiscard]] inline const Tile& at(const MapCoords& coords) const
    {
        return m_tiles[static_cast<std::size_t>(coords.y * width() + coords.x)];
    }

    [[nodiscard]] inline bool inBounds(const MapCoords& coords) const
    {
        return (coords.x >= 0 && coords.x < width() && coords.y >= 0 && coords.y < height());
    }

    bool load(const std::string& relativePath, const std::string& dataPath,
              const std::vector<TileInfo>& tilesInfo);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

  private:
    [[nodiscard]] const Tileset& findTileset(int globalId) const;
    bool loadTileLayer(int tileCount, const nlohmann::json& dataNode,
                       const std::vector<TileInfo>& tilesInfo);

    std::string m_name{};
    Dimensions m_dimension{};

    std::vector<Tile> m_tiles{};
    std::vector<Tileset> m_tilesets{};
    std::vector<GraphicsInfo> m_data{};
};

} // namespace cppRogue::environment

#endif
