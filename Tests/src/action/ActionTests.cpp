#include "catch2/catch.hpp"

#include "engine/action/Action.hpp"
#include "engine/action/Attack.hpp"
#include "engine/action/Rest.hpp"
#include "engine/entity/hero/Hero.hpp"
#include "engine/entity/monster/Monster.hpp"

/*
 Unit-Tests for Actions.
*/

// @TODO
// @TODO S'assurer que tous les tests passent
// @TODO

namespace cppRogue {

TEST_CASE("Can create a Rest action", "[Action]")
{
    action::Action restAction = action::Action{action::Rest()};

    SECTION("Rest is not bind to anything")
    {
        REQUIRE_FALSE(restAction->isEntity());
        REQUIRE_FALSE(restAction->asHero());
        REQUIRE_FALSE(restAction->asMonster());
    }

    SECTION("A Rest action is always a success")
    {
        const auto result = restAction->make();
        REQUIRE(result.isSuccess());
        REQUIRE(result.isDone());

        SECTION("A Rest action does not have an alternative action")
        {
            REQUIRE_FALSE(result.fallback);
        }
    }
}

TEST_CASE("Can create an Attack action", "[Action]")
{
    // Create the hero
    std::string name{"Dwarf"};
    std::string description{
        "Dwarf are small but stocky humanoid stone creature almost always bearded"};
    entity::Race::StatisticsMap stats{};
    stats[entity::Statistic::Strength] = 13;
    stats[entity::Statistic::Endurance] = entity::Hero::PunchPower;
    entity::Race race{name, description, stats};
    entity::HeroInfo info{"Dwarf", race};
    entity::Hero hero{sf::Vector2i{0, 1}, info};

    // Create a monster
    entity::Monster monster{sf::Vector2i{0, 1}, "Troll", 100, 2};

    // Create an attack action
    action::Action attackAction = action::Action{action::Attack(hero)};

    SECTION("Attack action is by default not bound to an entity")
    {
        REQUIRE_FALSE(attackAction->isEntity());
        REQUIRE_FALSE(attackAction->isHero());
        REQUIRE_FALSE(attackAction->isMonster());
    }

    SECTION("An Attack action can be bound")
    {
        SECTION("An Attack action can be bound to the Hero")
        {
            attackAction->bindHero(hero);
            REQUIRE(attackAction->isEntity());
            REQUIRE(attackAction->isHero());
            REQUIRE_FALSE(attackAction->isMonster());

            SECTION("Bounded entity is not a copy")
            {
                REQUIRE(hero.id() == attackAction->asHero()->id());
                REQUIRE(hero == *attackAction->asHero());
            }
        }

        SECTION("An Attack action can be bound to a monster")
        {
            attackAction->bindMonster(monster);
            REQUIRE(attackAction->isEntity());
            REQUIRE_FALSE(attackAction->isHero());
            REQUIRE(attackAction->isMonster());

            SECTION("Bounded entity is not a copy")
            {
                REQUIRE(monster.id() == attackAction->asMonster()->id());
                REQUIRE(monster == *attackAction->asMonster());
            }
        }
    }

    SECTION("Action owner and target can be the same")
    {
        // Bind the target as the owner
        attackAction->bindHero(hero);

        SECTION("Hero health is maximal")
        {
            REQUIRE(hero.maxHealth() == hero.health());
            REQUIRE(hero.isAlive());
        }

        const auto result = attackAction->make();
        REQUIRE(result.isSuccess());
        REQUIRE(result.isDone());

        SECTION("An Attack action does not have an alternative action")
        {
            REQUIRE_FALSE(result.fallback);
        }

        SECTION("Hero health has decreased")
        {
            REQUIRE_FALSE(hero.maxHealth() == hero.health());

            SECTION("Hero is dead")
            {
                REQUIRE(hero.health() == 0);
                REQUIRE_FALSE(hero.isAlive());
            }
        }
    }
}

} // namespace cppRogue
