#include "Inventory.hpp"

#include <cstddef>

namespace cppRogue::collectable {

Inventory::Inventory(Location location, std::size_t capacity)
    : m_location(location), m_capacity(capacity)
{
    m_inventoryStacks.reserve(capacity);
}

Inventory::AddResult Inventory::tryAdd(Item newItem, int count = 1)
{
    // @INFO
    // @INFO Ne pas dépasser la capacité maximale de "Inventory"
    // @INFO Ne pas dépasser la capacité maximale associé à chaque "Item
    // @INFO AddResult doit contenir
    // @INFO

    AddResult result;
    int remaining = count;

    for (auto&& stack : m_inventoryStacks)
    {
        if (stack.item.canBeStacked(newItem))
        {
            // Capacity = 10 || (8 + 4 => 10) et r=2 || (8 + 1 => 9) et r=-1 || (8 + 2 => 10) et r=0 
            remaining = (stack.count + count) - newItem.StackCapacity();

            // si r<=0 -> il ne m'en reste pas en rab, je rajoute le count, sinon j'ai atteint le max
            if (remaining <= 0) { 
                stack.count = stack.count + count;
                result.added = count;
            } else {
                stack.count = newItem.StackCapacity();
                result.added = stack.count - remaining;
            }

            break;
        }
    }

    if (remaining >= 0)
    {
        if (m_inventoryStacks.capacity() == m_inventoryStacks.size())  {
            result.added = 0;
        } else {
            m_inventoryStacks.emplace_back(ItemStack(count, newItem));
            result.added = count;
            result.remaining = 0;
        }
    }    

    return result;
}

Inventory::MergeResult Inventory::tryMerge(const Inventory& other)
{
    // @INFO
    // @INFO Pensez à réutilisez les autres méthodes
    // @INFO

    for (auto stack : other.m_inventoryStacks) { 
        AddResult res = tryAdd(stack.item, stack.count);
        stack.count -= res.added;
    }

    return Inventory::MergeResult{other};
}
std::optional<Item> Inventory::tryRemove(std::string_view itemName)
{
    std::optional<Item> removedItem = std::nullopt;

    int index = 0;
    for (auto& stack : m_inventoryStacks)
    {
        if (stack.item.name() == itemName)
        {
            removedItem = stack.item;
            m_inventoryStacks.erase(m_inventoryStacks.begin() + index);
            break;
        }
        index++;
    }

    return std::nullopt;
};

Inventory::AddResult::Status Inventory::AddResult::status() const
{
    auto status{AddResult::Status::Complete};

    if (added == 0)
    {
        // None added
        status = AddResult::Status::OutOfSpace;
    }
    else if (remaining > 0)
    {
        // At least one added
        status = AddResult::Status::Partial;
    }

    return status;
}

} // namespace cppRogue::collectable
