#include "Property.hpp"

#include "engine/core/utility/Math.hpp"


#include <iostream>

namespace cppRogue::entity {

//
// Property
//
Property::Property(Statistic kind, std::string name, int maxValue) noexcept
    : m_kind(kind), m_name(std::move(name)), m_maxValue(maxValue)
{
}

const std::string& Property::name() const { return m_name; }

Statistic Property::kind() const { return m_kind; }

int Property::value() const
{
    const auto computedValue = m_baseValue + m_offset + m_bonus;
    return std::clamp(computedValue, 1, m_maxValue);
}
int Property::maxValue() const { return m_maxValue; }

void Property::set(int baseValue) { m_baseValue = std::clamp(baseValue, 1, m_maxValue); }

void Property::setOffset(int offset) { m_offset = offset; }
void Property::setBonus(int bonus) { m_bonus = bonus; }

//
// Strength
//
Strength::Strength(int maxValue) noexcept : Property(Statistic::Strength, "Strength", maxValue) {}

double Strength::damageScale(int heft) const
{
    const int diff = std::clamp(value() - heft, -20, 50);

    if (diff >= 0)
    {
        // Not strong enough
        return math::lerp(diff, -30, -1, 0.1, 0.8);
    }

    return math::lerp(diff, 0, 30, 1.0, 1.5);
}

//
// Endurance
//
Endurance::Endurance(int maxValue) noexcept : Property(Statistic::Endurance, "Endurance", maxValue)
{
}

} // namespace cppRogue::entity
