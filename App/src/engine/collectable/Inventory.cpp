#include "Inventory.hpp"

#include <cstddef>

namespace cppRogue::collectable {

Inventory::Inventory(Location location, std::size_t capacity)
    : m_location(location), m_capacity(capacity)
{
    m_inventoryStacks.reserve(capacity);
}

std::size_t Inventory::count(const Item& item) const {
    
    int total = 0;
    for (auto&& stack : m_inventoryStacks) {
        if (item.name() == stack.item.name()) { 
            total += stack.count;
        }
    }

    return total;
}

Inventory::AddResult Inventory::tryAdd(Item newItem, int count = 1)
{
    if (count == 0) return AddResult{0, 0};

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
            if (count > newItem.StackCapacity())  { 
                result.added = newItem.StackCapacity();
                result.remaining = count - result.added; 
            } else {
                result.added = count;
                result.remaining = 0;
            }
            m_inventoryStacks.emplace_back(ItemStack(result.added, newItem));
        }
    }    

    return result;
}

Inventory::MergeResult Inventory::tryMerge(Inventory& other)
{
    for (auto& stack : other.m_inventoryStacks) { 
        AddResult res = tryAdd(stack.item, stack.count);
        stack.count -= res.added;
    }
    for (size_t i = 0; i < other.count(); i++) {
        if (other.m_inventoryStacks[i].count == 0) {
            other.tryRemove(other.m_inventoryStacks[i].item.name()); 
            i--;
        }
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
            if (stack.count > 1) 
                stack.count--;
            else 
                m_inventoryStacks.erase(m_inventoryStacks.begin() + index);

            break;
        }
        index++;
    }

    return removedItem;
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
