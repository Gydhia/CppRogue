#ifndef CPPROGUE_INVENTORY_HPP
#define CPPROGUE_INVENTORY_HPP

#include "engine/collectable/Item.hpp"

#include <optional>
#include <string_view>
#include <utility>
#include <vector>

// @TODO
// @TODO Compléter la classe "Inventory"
// @TODO L'API publique est fixée et ne doit pas être modifiée
// @TODO
//
// @TODO
// @TODO Compléter les tests dans `InventoryTests.cpp`
// @TODO

namespace cppRogue::collectable {

/**
 * @brief      Allowed locations for an inventory.
 */
enum class Location
{
    Ground,
    Bag,
    Shop
};

struct ItemStack
{
    ItemStack(int itemCount, Item stackedItem) : count(itemCount), item(std::move(stackedItem)) {}

    int count;
    Item item;
};

/**
 * @brief      An inventory is a collection of stacked items with a maximal number of
 *             stacks (capacity) and a specific location associated with it (location).
 *
 * @note       Each maximal stack size is item dependent.
 */
class Inventory
{
  public:
    // Forward definition
    struct MergeResult;
    struct AddResult;

    Inventory() = default;
    Inventory(Location location, std::size_t capacity);
    ~Inventory() = default;

    [[nodiscard]] inline Location location() const { return m_location; };

    /**
     * @brief      True when there is no items (all stacks are empty) inside the
     *             inventory.
     *
     * @return     True when the inventory is empty
     */
    [[nodiscard]] inline bool empty() const { return m_inventoryStacks.size() == 0; };

    /**
     * @brief      Maximal number of different stacks of items that can be handled by this
     *             inventory.
     *
     * @return     Number of stacks on this inventory
     */
    [[nodiscard]] inline std::size_t capacity() const { return m_inventoryStacks.capacity(); };

    /**
     * @brief      Total number of existing stacks in this inventory.
     *
     * @return     Number of stacks in the inventory
     */
    [[nodiscard]] inline std::size_t count() const { return m_inventoryStacks.size(); };

    /**
     * @brief      Stack size for this specific item. Will be zero if there is no items.
     *
     * @param[in]  item  The item to count
     *
     * @return     Number of similar items in this inventory
     */
    [[nodiscard]] std::size_t count(const Item& item) const;

    /**
     * @brief      Try to add @p newItem to this inventory in place.
     *
     * @param[in]  newItem  The item to be added
     * @param[in]  count    The size of the stack (how many item must be added)
     *
     * @return     The operation result
     */
    AddResult tryAdd(Item newItem, int count);

    /**
     * @brief      Merge items from @p other into this inventory.
     *
     * @param[in]  other  Other inventory
     *
     * @return     The merge result with remaining items if any
     */
    MergeResult tryMerge(Inventory& other);

    /**
     * @brief      Remove an Item from the inventory if possible.
     *
     * @param[in]  itemName  The item name
     *
     * @return     Item removed from the inventory and nullptr if nothing removed
     */
    std::optional<Item> tryRemove(std::string_view itemName);

  private:
    Location m_location{};
    std::size_t m_capacity{};

    std::vector<ItemStack> m_inventoryStacks;
};

/**
 * @brief      A wrapper around a merge result. Composed of both the operation result
 *             (state) and the items not merge during operation.
 */
struct Inventory::MergeResult
{
  public:
    enum class Status
    {
        Complete,
        Partial
    };

    explicit MergeResult(Inventory inventory) : remaining{inventory} {};
    MergeResult() = delete;
    ~MergeResult() = default;

    [[nodiscard]] inline bool isComplete() const { return status() == Status::Complete; }
    [[nodiscard]] inline Status status() const
    {
        return remaining.empty() ? Status::Complete : Status::Partial;
    };

    Inventory remaining;
};

/**
 * @brief      A wrapper around an add operation result. Composed of both the operation
 *             result (state) and the number of added and remaining items.
 */
struct Inventory::AddResult
{
  public:
    enum class Status
    {
        Complete,
        Partial,
        OutOfSpace
    };

    AddResult() = default;
    explicit AddResult(int addedCount, int remainingCount)
        : added(addedCount), remaining(remainingCount)
    {
    }
    ~AddResult() = default;

    [[nodiscard]] inline bool isComplete() const { return status() == Status::Complete; }
    [[nodiscard]] Status status() const;

    int added{0};
    int remaining{0};
};

} // namespace cppRogue::collectable

#endif
