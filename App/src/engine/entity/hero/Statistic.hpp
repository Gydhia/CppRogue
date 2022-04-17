#ifndef CPPROGUE_STATISTIC_HPP
#define CPPROGUE_STATISTIC_HPP

// @EXERCICE 1

#include "nlohmann/json.hpp"

namespace cppRogue::entity {

/**
 * @brief      Allowed statistics.
 */
enum class Statistic
{
    Strength,
    Endurance,
    Agility,
    Intellect
};


static constexpr int StatisticsCount = 4;

/* JSON serialization helper */
NLOHMANN_JSON_SERIALIZE_ENUM(Statistic, {{Statistic::Strength, "strength"},
                                         {Statistic::Endurance, "endurance"},
                                         {Statistic::Agility, "agility"},
                                         {Statistic::Intellect, "intellect"}});

} // namespace cppRogue::entity

#endif
