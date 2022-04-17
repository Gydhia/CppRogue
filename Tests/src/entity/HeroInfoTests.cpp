#include <catch2/catch.hpp>
#include <cstddef>

#include "engine/entity/hero/HeroInfo.hpp"

/*
 Unit-Tests for HeroInfo.
*/

namespace cppRogue {

TEST_CASE("HeroInfo creation", "[HeroInfo]")
{
    const std::string name{"Dwarf"};
    const std::string description{
        "Dwarf are small but stocky humanoid stone creature almost always bearded"};
    entity::Race::StatisticsMap stats{};
    stats[entity::Statistic::Strength] = 13;
    stats[entity::Statistic::Endurance] = 2;
    entity::Race race{name, description, stats};

    entity::HeroInfo info{"Manu", race};

    SECTION("Without any equipment armor and weight is zero")
    {
        REQUIRE(info.armor() == 0);
        REQUIRE(info.weight() == 0);
    }

    SECTION("Hero can gather gold")
    {
        const int goldAmount = 21;
        REQUIRE(info.gold() == 0);
        info.gatherGold(goldAmount);
        REQUIRE(info.gold() == goldAmount);

        SECTION("Negative numbers are discarded")
        {
            REQUIRE(info.gold() == goldAmount);
            info.gatherGold(-100);
            REQUIRE(info.gold() == goldAmount);
        }
    }

    SECTION("Hero can gather experience")
    {
        const int experienceAmount = 34;
        REQUIRE(info.experience() == 0);
        info.gatherExperience(experienceAmount);
        REQUIRE(info.experience() == experienceAmount);

        SECTION("Negative numbers are discarded")
        {
            REQUIRE(info.experience() == experienceAmount);
            info.gatherExperience(-100);
            REQUIRE(info.experience() == experienceAmount);
        }
    }

    SECTION("Hero armor / weight is based on equiped items")
    {
        auto appleData = collectable::ItemInfo{
            "Red Apple",
            0,
            1,
            0,
            1,
            1,
            "This one is juicy and red",
            {collectable::Slot::Chest, collectable::Slot::Helm, collectable::Slot::Necklace}};

        REQUIRE(info.weight() == 0);
        REQUIRE(info.armor() == 0);

        info.equipment().tryAdd(collectable::Item(appleData), collectable::Slot::Chest);
        REQUIRE(info.weight() == appleData.weight);
        REQUIRE(info.armor() == appleData.armor);

        info.equipment().tryAdd(collectable::Item(appleData), collectable::Slot::Helm);
        REQUIRE(info.weight() == appleData.weight * 2);
        REQUIRE(info.armor() == appleData.armor * 2);

        SECTION("Adding a weapon does not change the armor total amount")
        {
            const int oldArmor = info.armor();

            // Add a right hand weapon
            appleData.slots += collectable::Slot::RightHand;
            appleData.armor = 444;
            info.equipment().tryAdd(collectable::Item(appleData), collectable::Slot::RightHand);

            REQUIRE(info.armor() == oldArmor);
        }

        SECTION("Adding a weapon does change the weight total amount")
        {
            const int oldWeight = info.weight();

            // Add a right hand weapon
            appleData.slots += collectable::Slot::RightHand;
            appleData.weight = 123;
            info.equipment().tryAdd(collectable::Item(appleData), collectable::Slot::RightHand);

            REQUIRE_FALSE(info.weight() == oldWeight);
            REQUIRE(info.weight() == oldWeight + appleData.weight);
        }
    }
}

} // namespace cppRogue
