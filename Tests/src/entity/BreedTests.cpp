#include <catch2/catch.hpp>

#include "engine/core/AttackInfo.hpp"
#include "engine/entity/monster/Breed.hpp"

/*
 Unit-Tests for Breed.
*/

// @TODO
// @TODO Compléter le template de tests pour vérifier que votre implémentation est correcte
// @TODO Ce template est similaire à celui qui sera utilisé pour valider votre implémentation
// @TODO

namespace cppRogue {

TEST_CASE("Breed creation", "[Breed]")
{
    auto emptySpecs = cppRogue::entity::BreedInfo{};
    auto breed = cppRogue::entity::Breed(emptySpecs);

    SECTION("Can add attacks to a breed")
    {
        // @TODO
        // @TODO Implémenter le test
        REQUIRE(false);
        // @TODO Implémenter le test
        // @TODO
    }

    SECTION("Can add defenses to a breed")
    {
        // @TODO
        // @TODO Implémenter le test
        REQUIRE(false);
        // @TODO Implémenter le test
        // @TODO
    }

    SECTION("Can chain add methods (defenses or/and attacks)")
    {
        // @TODO
        // @TODO Implémenter le test
        REQUIRE(false);
        // @TODO Implémenter le test
        // @TODO
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

        // @TODO
        // @TODO Implémenter le test
        REQUIRE(false);
        // @TODO Implémenter le test
        // @TODO
    }
}

} // namespace cppRogue
