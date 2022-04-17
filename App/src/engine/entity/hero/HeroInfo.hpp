#ifndef CPPROGUE_HEROINFO_HPP
#define CPPROGUE_HEROINFO_HPP

#include "engine/collectable/Equipment.hpp"
#include "engine/core/Energy.hpp"
#include "engine/core/GraphicsInfo.hpp"
#include "engine/core/Motility.hpp"
#include "engine/entity/hero/Race.hpp"
#include "engine/entity/hero/Statistic.hpp"

namespace cppRogue::entity {

/**
 * @brief      Data container associated with an Hero. Allow to make a clear distinction
 *             between Hero temporary state and persistent state.
 */
class HeroInfo
{
  public:
    HeroInfo(std::string name, const Race& race);
    HeroInfo(std::string name, const Race& race, GraphicsInfo graphicsData);
    HeroInfo() = delete;
    ~HeroInfo() = default;

    // Pointer data members are not owned [-Weffc++]
    HeroInfo(const HeroInfo&) = default;
    HeroInfo(HeroInfo&&) = default;
    HeroInfo& operator=(const HeroInfo&) = default;
    // Pointer data members are not owned [-Weffc++]

    /* GETTERS */
    [[nodiscard]] int gold() const { return m_gold; };
    [[nodiscard]] int experience() const { return m_experience; };
    [[nodiscard]] int armor() const;
    [[nodiscard]] int weight() const;
    [[nodiscard]] inline collectable::Equipment& equipment() { return m_equipment; };

    /* SETTERS */
    void gatherGold(int amount) { m_gold += std::max(amount, 0); }
    void gatherExperience(int amount) { m_experience += std::max(amount, 0); }

  private:
    // Stats
    std::string m_name;
    const Race* m_race;
    Strength m_strength;
    Endurance m_endurance;

    Motilities m_motilities{Motility::Walk};
    int m_speed{Energy::NormalSpeed};

    // Gains
    int m_experience{};
    int m_gold{};
    collectable::Equipment m_equipment{};

    // Graphics
    GraphicsInfo m_graphicsData{{}}; // Default to empty texture

    // Special private access to Hero for convenience
    friend class Hero;
};

} // namespace cppRogue::entity

#endif
