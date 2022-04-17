#include "catch2/catch.hpp"

#include "engine/collectable/Item.hpp"
#include "engine/core/AttackInfo.hpp"

/*
 Unit-Tests for Items.
*/

namespace cppRogue {

TEST_CASE("Can create an Item", "[Item]")
{
    collectable::Item item = collectable::Item{};
    collectable::ItemInfo itemData = collectable::ItemInfo{
        "Great Sword", 10, 100, 23, 21, 1, "Forged inside a volcano by the great dwarf"};

    SECTION("Creation of an Item create a deep copy of an ItemInfo")
    {
        collectable::Item greateSword{itemData};

        // update item data
        itemData.name = "Wood stick";
        itemData.price = 1;
        REQUIRE(itemData.name != greateSword.name());
        REQUIRE(itemData.price != greateSword.price());

        SECTION("Two different item cannot be stacked")
        {
            REQUIRE(greateSword.canBeStacked(greateSword));

            collectable::Item fakeSword{itemData};
            REQUIRE(greateSword.canBeStacked(fakeSword) == false);
        }
    }

    SECTION("An ItemInfo can have an optional collection of available slots")
    {
        auto first = collectable::ItemInfo{};
        auto second = collectable::ItemInfo{};
        REQUIRE(first == second);

        second.slots = {collectable::Slot::Boots, collectable::Slot::LeftHand};
        REQUIRE(first != second);

        first.slots |= collectable::Slot::Boots;    // Same as +=
        first.slots |= collectable::Slot::LeftHand; // Same as |=
        REQUIRE(first == second);
    }

    SECTION("An ItemInfo can have an optional attack information")
    {
        auto first = collectable::ItemInfo{};
        auto second = collectable::ItemInfo{};
        REQUIRE(first == second);

        second.attack = AttackInfo{"Super punch", 10};
        REQUIRE(first != second);

        first.attack = AttackInfo{"Super punch", 10};
        REQUIRE(first == second);
    }
}

} // namespace cppRogue
