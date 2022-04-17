#ifndef CPPROGUE_MATH_HPP
#define CPPROGUE_MATH_HPP

#include "Debug.hpp"

#include <SFML/System/Vector2.hpp>

#include <algorithm> // std::max
#include <cmath>     // std::abs

namespace cppRogue::math {

/* Remaps @p value (within the range @p min to @p max) in the output range (@p outMin to
 * @p outMax).
 *
 * @param[in]  value   The value
 * @param[in]  inMin     The minimum
 * @param[in]  inMax     The maximum
 * @param[in]  outMin  The output minimum
 * @param[in]  outMax  The output maximum
 *
 * @return     Value in the output range (@p outMin to @p outMax).
 */
inline double lerp(int value, int inMin, int inMax, double outMin, double outMax)
{
    ASSERT_DEBUG(inMin < inMax, "inMin should be lower than inMax ...");
    if (value <= inMin) return outMin;
    if (value >= inMax) return outMax;

    // Reduce total error by first computing 't' ...
    double t = (value - inMin) / static_cast<double>(inMax - inMin); // NOLINT
    // ... then using 't' and '1.0 - t'
    double result = outMin * (1.0 - t) + outMax * t;

    return result;
}

/**
 * @brief      Number of square a king on a chessboard would need to move to reach @p
 *             target starting at @p origin.
 *
 * @param[in]  origin  Current position
 * @param[in]  target  Target position
 *
 * @return     The king length.
 */
inline int chebyshevDistance(sf::Vector2i origin, sf::Vector2i target)
{
    auto diff = target - origin;
    return std::max(std::abs(diff.x), std::abs(diff.y));
}

} // namespace cppRogue::math

#endif
