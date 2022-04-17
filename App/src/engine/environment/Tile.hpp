#ifndef CPPROGUE_TILE_HPP
#define CPPROGUE_TILE_HPP

#include "TileInfo.hpp"
#include "engine/core/Motility.hpp"

namespace cppRogue::environment {

/**
 * @brief      An unit element of a map. A tile has an associated TileInfo that describes
 *             its characteristics for both rendering and pathfinding.
 */
class Tile
{
  public:
    explicit Tile(const TileInfo& tileInfo);
    ~Tile() = default;

    /**
     * @brief      True if there is at least one motility in common between this tile and
     *             @p entityMobilities.
     *
     * @param[in]  entityMotilities  The motilities associated with an entity that want to
     *                               step in.
     *
     * @return     True if able to enter, False otherwise.
     */
    [[nodiscard]] bool canEnter(Motilities entityMotilities) const;

    /**
     * @brief      Get the TileInfo name.
     *
     * @return     Corresponding TileInfo name
     */
    [[nodiscard]] inline std::string_view name() const { return m_info.name; }

    /**
     * @brief      Get the TileInfo id.
     *
     * @return     Corresponding TileInfo id
     */
    [[nodiscard]] inline int id() const { return m_info.id; }

    /**
     * @brief      Get theTileInfo associated cost.
     *
     * @return     Corresponding TileInfo cost.
     */
    [[nodiscard]] inline int cost() const { return m_info.cost; }

  private:
    const TileInfo& m_info;
};

} // namespace cppRogue::environment

#endif
