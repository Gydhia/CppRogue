#ifndef CPPROGUE_ALGORITHM_HPP
#define CPPROGUE_ALGORITHM_HPP

#include <SFML/System/Vector2.hpp>

#include <algorithm>
#include <string>
#include <vector>

namespace cppRogue::algorithm {

/**
 * @brief      Create a copy of @p str with all characters transformed to lower case.
 *
 * @param[in]  str   Input string
 *
 * @return     Lower case version of @p str
 */
inline std::string lower(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c) -> unsigned char { return std::tolower(c); });
    return str;
}

} // namespace cppRogue::algorithm
#endif
