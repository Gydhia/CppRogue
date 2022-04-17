#ifndef CPPROGUE_SLOT_HPP
#define CPPROGUE_SLOT_HPP

#include "engine/core/utility/Bitflags.hpp"

#include "nlohmann/json.hpp"

namespace cppRogue::collectable {

enum class Slot
{
    LeftHand,
    RightHand,
    Helm,
    Necklace,
    Chest,
    Boots
};

static constexpr int SlotsCount = 6;

/**
 * @brief      Bitflags of all allowed Slots.
 */
using SlotFlags = Bitflags<Slot, SlotsCount>;

/* JSON serialization helper */
NLOHMANN_JSON_SERIALIZE_ENUM(Slot, {{Slot::LeftHand, "lefthand"},  // Default
                                    {Slot::RightHand, "righthand"},
                                    {Slot::Helm, "helm"},
                                    {Slot::Necklace, "necklace"},
                                    {Slot::Chest, "chest"},
                                    {Slot::Boots, "boots"}});

} // namespace cppRogue::collectable

#endif
