#include <catch2/catch.hpp>

#include "engine/core/AttackInfo.hpp"
#include "engine/entity/monster/Breed.hpp"
#include <content/ContentManager.hpp>

/*
 Unit-Tests for Breed.
*/

namespace cppRogue {

TEST_CASE("Breed creation", "[Breed]")
{
    auto emptySpecs = cppRogue::entity::BreedInfo{};
    auto breed = cppRogue::entity::Breed(emptySpecs);

    SECTION("Can add attacks to a breed")
    {
        AttackInfo funAttack = AttackInfo{"Fun", 1};
        breed.add(funAttack);

        REQUIRE(breed.attacks().size() > 0);
    }

    SECTION("Can add defenses to a breed")
    {
        cppRogue::entity::DefenseInfo funDefense = cppRogue::entity::DefenseInfo{"Anti-weebs", 1, 1, 500};
        breed.add(funDefense);

        REQUIRE(breed.defenses().size() > 0);
    }

    SECTION("Can chain add methods (defenses or/and attacks)")
    {
        AttackInfo funAttack = AttackInfo{"ILikeTrains", 1};
        cppRogue::entity::DefenseInfo funDefense = cppRogue::entity::DefenseInfo{"Anti-simps", 500, 1, 0};

        breed.add(funAttack).add(funDefense);
        bool chainAdded = breed.defenses().size() == 1 && breed.attacks().size() == 1;   

        REQUIRE(chainAdded);
    }
}

TEST_CASE("Breed data", "[Breed]")
{
    SECTION("Public API allowed to access all breed data (see list in Breed.json) except graphics")
    {
        auto specs = cppRogue::entity::BreedInfo{};
        specs.name = "CSharp";
        specs.description = "A language that should be thrown into a deep abyss !";
        specs.maxHealth = 10;
        specs.speed = 2;
        specs.dodge = 3;
        specs.experience = 1;
        specs.trackingDistance = 2;
        specs.motilities = {Motility::Swim};

        auto breed = cppRogue::entity::Breed(specs);

        ContentManager gameData{};
        if (!gameData.load("data/content.json"))
        {
            std::cerr << "Game content loading failed !" << '\n';
            return;
        }

        for (auto breed : gameData.breeds()) { 
            if (breed.get()->data().name == "") 
                REQUIRE(false);
        }
        REQUIRE(true);
    }
}
} // namespace cppRogue
