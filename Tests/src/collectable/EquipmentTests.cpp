#include "catch2/catch.hpp"

#include "engine/collectable/Equipment.hpp"
#include "engine/collectable/Item.hpp"

/*
 Unit-Tests for Equipment.
*/

namespace cppRogue {

TEST_CASE("Can create an Equipment", "[Equipment]")
{
    collectable::Equipment equip{};

    SECTION("Can access equipment state")
    {
        REQUIRE(equip.count() == 0);
        for (int slotIndex = 0; slotIndex < collectable::SlotsCount; ++slotIndex)
        {
            REQUIRE(equip.available(static_cast<collectable::Slot>(slotIndex)));
            REQUIRE_FALSE(equip.used(static_cast<collectable::Slot>(slotIndex)));
        }
    }

    SECTION("Can test if an equipment can be added")
    {
        auto testData = collectable::ItemInfo{};

        // By default an item cannot be added as an equipment
        REQUIRE_FALSE(collectable::Equipment::canBeEquipped(collectable::Item(testData)));

        // If at least one slot is available then it is possible
        testData.slots += collectable::Slot::LeftHand;
        REQUIRE(collectable::Equipment::canBeEquipped(collectable::Item(testData)));
    }

    SECTION("Can add new equipment")
    {
        auto swordData = collectable::ItemInfo{
            "Great Sword", 10, 100, 23, 21, 1, "Forged inside a volcano by the great dwarf"};
        auto appleData = collectable::ItemInfo{
            "Red Apple", 0, 1, 0, 1, 1, "This one is juicy and red", {collectable::Slot::LeftHand}};

        REQUIRE(appleData.slots.enabled(collectable::Slot::LeftHand));

        auto result = equip.tryAdd(collectable::Item(appleData), collectable::Slot::LeftHand);
        REQUIRE(result.isAdded());
        REQUIRE(equip.used(collectable::Slot::LeftHand));

        // If item does not have the right slot it cannot be added
        result = equip.tryAdd(collectable::Item(swordData), collectable::Slot::RightHand);
        REQUIRE_FALSE(result.isAdded());
        REQUIRE_FALSE(result.old.has_value());
        REQUIRE(equip.available(collectable::Slot::RightHand));

        // If item has the right slot it can be added
        swordData.slots = {collectable::Slot::RightHand, collectable::Slot::LeftHand};
        result = equip.tryAdd(collectable::Item(swordData), collectable::Slot::RightHand);
        REQUIRE(result.isAdded());
        REQUIRE_FALSE(result.old.has_value());
        REQUIRE(equip.used(collectable::Slot::RightHand));

        SECTION("Adding a new item to an used slot returns the old item equiped")
        {
            const auto bananaData = collectable::ItemInfo{
                "Banana", 0, 1, 0, 1, 4, "A fruit usually elongated and curved"};

            REQUIRE(equip.used(collectable::Slot::LeftHand));
            auto result = equip.tryAdd(collectable::Item(appleData), collectable::Slot::LeftHand);
            REQUIRE(equip.used(collectable::Slot::LeftHand));
            REQUIRE(result.isAdded());
            REQUIRE(result.old.has_value());
            REQUIRE(result.old.value().canBeStacked(collectable::Item(appleData)));
        }

        SECTION("An equiped item can be removed")
        {
            REQUIRE(equip.used(collectable::Slot::LeftHand));
            auto oldItem = equip.removeAt(collectable::Slot::LeftHand);
            REQUIRE(equip.available(collectable::Slot::LeftHand));
        }

        SECTION("Trying to remove an item from an available slot throws an exception")
        {
            REQUIRE(equip.available(collectable::Slot::Boots));
            REQUIRE_THROWS(equip.removeAt(collectable::Slot::Boots));
        }
    }
}

TEST_CASE("Can iterate over equiped items", "[Equipment]")
{
    const auto swordData =
        collectable::ItemInfo{"Great Sword",
                              10,
                              100,
                              23,
                              21,
                              1,
                              "Forged inside a volcano by the great dwarf",
                              {collectable::Slot::RightHand, collectable::Slot::LeftHand}};

    const auto appleData = collectable::ItemInfo{
        "Red Apple",
        0,
        1,
        0,
        1,
        1,
        "This one is juicy and red",
        {collectable::Slot::Chest, collectable::Slot::Helm, collectable::Slot::Necklace}};

    // Add some equipments
    collectable::Equipment equip{};
    equip.tryAdd(collectable::Item(appleData), collectable::Slot::Chest);
    equip.tryAdd(collectable::Item(appleData), collectable::Slot::Helm);
    equip.tryAdd(collectable::Item(appleData), collectable::Slot::Necklace);
    equip.tryAdd(collectable::Item(swordData), collectable::Slot::RightHand);
    equip.tryAdd(collectable::Item(swordData), collectable::Slot::LeftHand);

    SECTION("Can iterate over all weapons")
    {
        int counter{};
        for (const auto* weapon : equip.weapons()) { counter++; }
        REQUIRE(counter == 2);
    }

    SECTION("Can iterate over all armors")
    {
        int counter{};
        for (const auto* armor : equip.armors()) { counter++; }

        // LeftHand is at the same time an armor and a weapon
        REQUIRE(counter == 4);
    }

    SECTION("Can iterate over all equipments")
    {
        int counter{};
        for (const auto* any : equip.all()) { counter++; }
        REQUIRE(counter == 5);

        SECTION("Can count the current total number of equipments")
        {
            REQUIRE(counter == equip.count());
        }
    }
}

} // namespace cppRogue
