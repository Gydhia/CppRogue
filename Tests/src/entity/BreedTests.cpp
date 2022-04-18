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
    auto emptyGraphics = GraphicsInfo{sf::Texture{}};
    auto breed = cppRogue::entity::Breed(emptySpecs, emptyGraphics);

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
        ContentManager gameData{};
        //REQUIRE(gameData.load("data/content.json"));

        std::vector<cppRogue::entity::Breed> breeds;
        for (auto breed : gameData.breeds()) { 
            breeds.emplace_back(*breed);
            if (breed.get()->data().name == "") 
                REQUIRE(false);
        }

        REQUIRE(true);
    }
}
} // namespace cppRogue
