#ifndef CPPROGUE_EQUIPMENT_HPP
#define CPPROGUE_EQUIPMENT_HPP

#include "engine/collectable/Item.hpp"
#include "engine/collectable/Slot.hpp"

#include <optional>
#include <unordered_map>
#include <vector>

namespace cppRogue::collectable {

class Equipment
{
  public:
    struct EquipResult
    {
      public:
        enum class Status
        {
            Discarded,
            Added
        };

        EquipResult() = delete;
        explicit EquipResult(Status operationResult) : status(operationResult) {}

        [[nodiscard]] inline bool isAdded() const { return status == Status::Added; }

        Status status{};
        std::optional<Item> old{};
    };

    Equipment();
    ~Equipment() = default;

    [[nodiscard]] std::size_t count() const
    {
        std::size_t equipped = m_status.count();
        return equipped;
    }
    [[nodiscard]] std::vector<const Item*> weapons() const;
    [[nodiscard]] std::vector<const Item*> armors() const;
    [[nodiscard]] std::vector<const Item*> all() const;

    [[nodiscard]] bool available(Slot slot) const { return m_status.disabled(slot); }
    [[nodiscard]] bool used(Slot slot) const { return m_status.enabled(slot); }
    static bool canBeEquipped(const Item& item) { return item.usableSlots().any(); }

    EquipResult tryAdd(Item item, Slot slot);
    Item removeAt(Slot slot);

  private:
    std::unordered_map<Slot, Item> m_slots{};
    SlotFlags m_status{};

    // 'mutable' allows to update 'm_buffer' even in a method marked as const
    // This does not break constness because our instance logical state is not altered
    // More information (french) https://blog.pcitron.fr/2011/02/23/c-et-mutable/
    mutable std::vector<const Item*> m_buffer{};
};

} // namespace cppRogue::collectable

#endif
