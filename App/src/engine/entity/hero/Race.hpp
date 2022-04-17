#ifndef CPPROGUE_RACE_HPP
#define CPPROGUE_RACE_HPP

#include "Property.hpp"
#include "Statistic.hpp"

#include <string_view>
#include <unordered_map>

namespace cppRogue::entity {

/**
 * @brief      Race data container.
 */
class Race
{
  public:
    /// Type alias for a hash table of statistic --> value
    using StatisticsMap = std::unordered_map<entity::Statistic, int>;

    Race(std::string name, std::string description, const StatisticsMap& statistics);
    Race() = delete;
    ~Race() = default;

    [[nodiscard]] inline std::string_view name() const { return m_name; }
    [[nodiscard]] inline std::string_view description() const { return m_description; }
    [[nodiscard]] int property(Statistic kind) const;

  private:
    std::string m_name{};
    std::string m_description{};
    StatisticsMap m_statistics{};
};

} // namespace cppRogue::entity

#endif
