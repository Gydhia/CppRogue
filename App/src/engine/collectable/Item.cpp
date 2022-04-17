#include "Item.hpp"

namespace cppRogue::collectable {

Item::Item(const ItemInfo& info) : m_data{info} {}

bool ItemInfo::operator==(const ItemInfo& rhs) const
{
    return (name == rhs.name) && (armor == rhs.armor) && (price == rhs.price) &&
           (weight == rhs.weight) && (heft == rhs.heft) && (fuel == rhs.fuel) &&
           (slots == rhs.slots) && (attack == rhs.attack);
}

bool ItemInfo::operator!=(const ItemInfo& rhs) const { return !operator==(rhs); }

} // namespace cppRogue::collectable
