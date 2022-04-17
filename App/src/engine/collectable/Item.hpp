#ifndef CPPROGUE_ITEM_HPP
#define CPPROGUE_ITEM_HPP

#include "Slot.hpp"
#include "engine/core/AttackInfo.hpp"

#include <cstddef>
#include <string>
#include <string_view>

namespace cppRogue::collectable {

/**
 * @brief      Data only container for an item.
 */
struct ItemInfo
{
  public:
    ItemInfo() = default;
    ~ItemInfo() = default;

    bool operator==(const ItemInfo& rhs) const;
    bool operator!=(const ItemInfo& rhs) const;

    std::string name{"None"};

    /// How much armor this item provide
    int armor{0};

    /// How much gold this item is worth.
    int price{0};

    /// The penalty to the hero's strength when wearing this.
    int weight{0};

    /// The amount of strength required to wield the item effectively.
    int heft{0};

    /// How much this item provided fuel when set on fire
    int fuel{0};

    /// Lore only
    std::string description{"None"};

    /// Where this item can be equiped
    SlotFlags slots{};

    /// Attack information (kind, damage, ...)
    AttackInfo attack{};

    /// Max size of a stack
    static constexpr std::size_t StackCapacity{10};
};

/**
 * @brief      A Item is just an interface to its data.
 */
class Item
{
  public:
    Item() = default;
    explicit Item(const ItemInfo& info);
    ~Item() = default;

    [[nodiscard]] inline std::string_view name() const { return m_data.name; }
    [[nodiscard]] inline int armor() const { return m_data.armor; }
    [[nodiscard]] inline int weight() const { return m_data.weight; }
    [[nodiscard]] inline int fuel() const { return m_data.fuel; }
    [[nodiscard]] inline int price() const { return m_data.price; }
    [[nodiscard]] inline int heft() const { return m_data.heft; }

    [[nodiscard]] bool canBeStacked(const Item& other) const { return m_data == other.m_data; }
    [[nodiscard]] static constexpr std::size_t StackCapacity() { return ItemInfo::StackCapacity; }

    [[nodiscard]] inline SlotFlags usableSlots() const { return m_data.slots; }
    [[nodiscard]] inline const AttackInfo& attackInfo() const { return m_data.attack; }

  private:
    ItemInfo m_data{};
};

} // namespace cppRogue::collectable

#endif
