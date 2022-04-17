#include "Race.hpp"

#include "Property.hpp"
#include "Statistic.hpp"

#include "engine/core/utility/Debug.hpp"

#include <iostream>

namespace cppRogue::entity {

Race::Race(std::string name, std::string description, const StatisticsMap& statistics)
    : m_name(std::move(name)), m_description(std::move(description))
{
    // Make sure all field are set to avoid exception at runtime later
    for (int stat = 0; stat < StatisticsCount; ++stat)
    {
        m_statistics[static_cast<Statistic>(stat)] = 1;
    }

    // Override default values with provided.
    for (const auto& [key, value] : statistics) { m_statistics[key] = value; }
}

int Race::property(const Statistic kind) const { return m_statistics.at(kind); }

} // namespace cppRogue::entity
