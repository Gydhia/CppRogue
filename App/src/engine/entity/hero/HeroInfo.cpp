#include "HeroInfo.hpp"
#include "engine/entity/hero/Statistic.hpp"

namespace cppRogue::entity {

HeroInfo::HeroInfo(std::string name, const Race& race)
    : m_name{std::move(name)},
      m_race{&race},
      m_strength(Strength(race.property(Statistic::Strength))),
      m_endurance(Endurance(race.property(Statistic::Endurance)))
{
}

HeroInfo::HeroInfo(std::string name, const Race& race, GraphicsInfo graphicsData)
    : m_name{std::move(name)},
      m_race{&race},
      m_strength(Strength(race.property(Statistic::Strength))),
      m_endurance(Endurance(race.property(Statistic::Endurance))),
      m_graphicsData(std::move(graphicsData))
{
}

int HeroInfo::armor() const
{
    int armor = 0;
    for (auto&& item : m_equipment.armors())
    {
        // Cannot be nullptr
        armor += item->armor();
    }

    return armor;
}

int HeroInfo::weight() const
{
    int weight = 0;
    for (auto&& item : m_equipment.all())
    {
        // Cannot be nullptr
        weight += item->weight();
    }

    return weight;
}

} // namespace cppRogue::entity
