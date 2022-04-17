#include "Energy.hpp"

#include "utility/Debug.hpp"

#include <algorithm> // std::clamp

namespace cppRogue {

Energy::Energy(int startAmount) : m_amount(startAmount) {}

void Energy::accumulate(int speed)
{
    // From speed to energy amount using a fast table lookup
    const int clampedSpeed = std::clamp(speed, MinSpeed, MaxSpeed);
    const int energyGain = SpeedTable[clampedSpeed];
    m_amount += energyGain;
}

bool Energy::canTakeTurn() const { return m_amount >= ActionThreshold; }

void Energy::spend()
{
    ASSERT_DEBUG(canTakeTurn(),
                 "Spend should only be called when energy level is above ActionThreshold");

    m_amount -= ActionThreshold;
}

} // namespace cppRogue
