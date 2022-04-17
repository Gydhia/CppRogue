#include "catch2/catch.hpp"

#include "engine/collectable/Inventory.hpp"

/*
 Unit-Tests for Inventory.
*/

// @TODO
// @TODO Compléter le template de tests pour vérifier que votre implémentation est correcte
// @TODO Ce template est similaire à celui qui sera utilisé pour valider votre implémentation
// @TODO

namespace cppRogue {

TEST_CASE("Can create an Inventory", "[Inventory]")
{
    const std::size_t bagCapacity = 2;

    collectable::Inventory bag{collectable::Location::Bag, bagCapacity};

    SECTION("Can access inventory current state")
    {
        bool testPassed = bag.location() == collectable::Location::Bag && bag.capacity() == bagCapacity;
        REQUIRE(testPassed);
    }
}

TEST_CASE("An item can be added to the inventory", "[Inventory]")
{
    const std::size_t bagCapacity = 2;
    collectable::Inventory shop{collectable::Location::Shop, bagCapacity};

    const auto swordData = collectable::ItemInfo{
        "Great Sword", 10, 100, 23, 21, 1, "Forged inside a volcano by the great dwarf"};
    const auto apple =
        collectable::ItemInfo{"Red Apple", 0, 1, 0, 1, 1, "This one is juicy and red"};
    const auto banana =
        collectable::ItemInfo{"Banana", 0, 1, 0, 1, 4, "A fruit usually elongated and curved"};

    SECTION("Adding an item can be fully completed")
    {
        cppRogue::collectable::Item sword = cppRogue::collectable::Item{swordData};
        cppRogue::collectable::Inventory::AddResult result = shop.tryAdd(sword, 1);
        
        if (result.isComplete() && result.added == 1)
            REQUIRE(true);
        else
            REQUIRE(false);

        SECTION("Adding an item can be partial if item max stack is reached")
        {
            cppRogue::collectable::Item bananaItem = cppRogue::collectable::Item{banana};
            cppRogue::collectable::Inventory::AddResult bananaResult = shop.tryAdd(bananaItem, 12);
            
            if (bananaResult.status() == cppRogue::collectable::Inventory::AddResult::Status::Partial)
                REQUIRE(true);
            else
                REQUIRE(false);

            SECTION("Adding an item can fail if there is not enough available slots")
            {
                cppRogue::collectable::Item appleItem = cppRogue::collectable::Item{apple};
                cppRogue::collectable::Inventory::AddResult appleResult = shop.tryAdd(appleItem, 12);
                
                if (appleResult.status() == cppRogue::collectable::Inventory::AddResult::Status::OutOfSpace)
                    REQUIRE(true);
                else
                    REQUIRE(false);
            }
        }
    }
}

TEST_CASE("An Item can be removed from the inventory", "[Inventory]")
{
    const std::size_t bagCapacity = 2;
    collectable::Inventory shop{collectable::Location::Shop, bagCapacity};

    const auto swordData = collectable::ItemInfo{
        "Great Sword", 10, 100, 23, 21, 1, "Forged inside a volcano by the great dwarf"};
    collectable::Item sword{swordData};
    shop.tryAdd(collectable::Item{swordData}, 2);

    SECTION("Removing an item does not remove the whole stack if still an item inside")
    {
        std::optional<cppRogue::collectable::Item> removedItem = shop.tryRemove(sword.name());
        if (removedItem != std::nullopt && shop.count() == 1)
            REQUIRE(true);
        else
            REQUIRE(false);

        SECTION("The whole stack is removed when last item of the stack is removed")
        {
            removedItem = shop.tryRemove(sword.name());
            if (removedItem != std::nullopt && shop.count() == 0)
                REQUIRE(true);
            else
                REQUIRE(false);

            SECTION("Trying to remove an item that does not exist is safe")
            {
                removedItem = shop.tryRemove(sword.name());
                if (removedItem == std::nullopt)
                    REQUIRE(true);
                else
                    REQUIRE(false);
            }
        }
    }
}

TEST_CASE("Two Inventory can be merged together", "[Inventory]")
{
    const auto swordData = collectable::ItemInfo{
        "Great Sword", 10, 100, 23, 21, 1, "Forged inside a volcano by the great dwarf"};
    const auto appleData =
        collectable::ItemInfo{"Red Apple", 0, 1, 0, 1, 1, "This one is juicy and red"};
    const auto bananaData =
        collectable::ItemInfo{"Banana", 0, 1, 0, 1, 4, "A fruit usually elongated and curved"};

    collectable::Inventory bag{collectable::Location::Bag, 3};
    collectable::Inventory lootOnGround{collectable::Location::Ground, 2};

    // Fill bag inventory
    auto addResult = bag.tryAdd(collectable::Item{bananaData}, 5);
    addResult = bag.tryAdd(collectable::Item{appleData}, 5);

    // Fill lootOnGround inventory
    addResult = lootOnGround.tryAdd(collectable::Item{swordData}, 2);
    addResult = lootOnGround.tryAdd(collectable::Item{appleData}, 4);

    SECTION("A merge is a success if there is enough room")
    {
        cppRogue::collectable::Inventory::MergeResult mResult = bag.tryMerge(lootOnGround);
        if (mResult.isComplete())
            REQUIRE(true);
        else
            REQUIRE(false);

        SECTION("A merge is a failure if there is not enough room")
        {
            addResult = lootOnGround.tryAdd(collectable::Item{swordData}, 2);
            addResult = lootOnGround.tryAdd(collectable::Item{appleData}, 4);

            mResult = bag.tryMerge(lootOnGround);

            if (mResult.status() == cppRogue::collectable::Inventory::MergeResult::Status::Complete)
                REQUIRE(false);
            else
                REQUIRE(true);
        }
    }
}

} // namespace cppRogue
