#ifndef CPPROGUE_MOTILITY_HPP
#define CPPROGUE_MOTILITY_HPP

#include "engine/core/utility/Bitflags.hpp"

#include "nlohmann/json.hpp"

namespace cppRogue {
/**
 * @brief      Each tile and entities have a set of motilities that describe respectively
 *             how they can move and the requirements to move to that tile.
 */
enum class Motility
{
    Walk,
    Swim,
    Fly
};

/**
 * @brief      Bitflags of all allowed motilities.
 */
using Motilities = Bitflags<Motility, 3>;

/* JSON serialization helper */
NLOHMANN_JSON_SERIALIZE_ENUM(Motility, {
                                           {Motility::Walk, "walk"},
                                           {Motility::Swim, "swim"},
                                           {Motility::Fly, "fly"},
                                       })

} // namespace cppRogue

#endif
