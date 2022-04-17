#include "Equipment.hpp"

#include "engine/collectable/Slot.hpp"

namespace cppRogue::collectable {

Equipment::Equipment()
{
    // Allocation of enough memory to handle the worst case
    m_buffer.reserve(m_status.count());
}

Item Equipment::removeAt(Slot slot)
{
    auto equipment = m_slots.at(slot);
    m_slots.erase(slot);
    m_status -= slot;

    return equipment;
}

Equipment::EquipResult Equipment::tryAdd(Item item, Slot slot) // NOLINT (copy to get ownership)
{
    if (item.usableSlots().disabled(slot)) return EquipResult{EquipResult::Status::Discarded};

    // Store a copy of the old one
    EquipResult result{EquipResult::Status::Added};
    if (used(slot)) { result.old = m_slots.at(slot); }

    m_slots[slot] = std::move(item);
    m_status += slot;

    return result;
}

std::vector<const Item*> Equipment::weapons() const
{
    m_buffer.clear();

    // Initialization list make it easy
    auto weaponTypes = SlotFlags{Slot::LeftHand, Slot::RightHand};

    for (auto&& [type, item] : m_slots)
    {
        const auto isWeapon = weaponTypes.enabled(type);
        if (isWeapon && used(type)) { m_buffer.emplace_back(&item); }
    }

    return m_buffer;
}

std::vector<const Item*> Equipment::armors() const
{
    m_buffer.clear();

    auto armorTypes =
        SlotFlags{Slot::Boots, Slot::Chest, Slot::Helm, Slot::Necklace, Slot::LeftHand};
    for (auto&& [type, item] : m_slots)
    {
        const auto isArmor = armorTypes.enabled(type);
        if (isArmor && used(type)) { m_buffer.emplace_back(&item); }
    }

    return m_buffer;
};

std::vector<const Item*> Equipment::all() const
{
    m_buffer.clear();

    for (auto&& [type, item] : m_slots)
    {
        if (used(type)) { m_buffer.emplace_back(&item); }
    }

    return m_buffer;
};

} // namespace cppRogue::collectable
