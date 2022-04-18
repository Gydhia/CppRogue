#include <catch2/catch.hpp>

#include "engine/entity/hero/Hero.hpp"
#include "engine/entity/monster/Monster.hpp"

#include <iostream>

namespace cppRogue {

TEST_CASE("Can create an Entity (Monster or Hero)", "[Entity]")
{
    entity::Breed monsterBreed = entity::Breed{entity::BreedInfo{
        "Troll", "An ugly shit", 3, 100, Energy::NormalSpeed, 1, 3, Motilities{Motility::Walk}, 5},
        GraphicsInfo{sf::Texture{}}};
    entity::Monster monster{sf::Vector2i{0, 1}, monsterBreed};

    std::string name{"Dwarf"};
    std::string description{
        "Dwarf are small but stocky humanoid stone creature almost always bearded"};
    entity::Race::StatisticsMap stats{};
    stats[entity::Statistic::Strength] = 13;
    stats[entity::Statistic::Endurance] = 2;

    entity::Race race{name, description, stats};
    entity::HeroInfo info{"Dwarf", race};
    entity::Hero hero{sf::Vector2i{0, 1}, info};

    SECTION("Starting health is maximum health")
    {
        REQUIRE(monster.health() == monster.maxHealth());
        REQUIRE(hero.health() == hero.maxHealth());
    }

    SECTION("Stream operator is overloaded")
    {
        std::cout << monster << '\n';
        std::cout << hero << '\n';
    }

    SECTION("Can compare two entities using equality/inequality operators")
    {
        REQUIRE(monster == monster);
        entity::Monster anotherMonster = monster; // NOLINT
        REQUIRE(monster != anotherMonster);

        REQUIRE(hero == hero);
        entity::Hero anotherHero = hero; // NOLINT
        REQUIRE(hero != anotherHero);

        // Reference are always the same
        const entity::Monster& monsterRef = monster;
        const entity::Hero& heroRef = hero;
        REQUIRE(monsterRef == monster);
        REQUIRE(heroRef == hero);
    }
}

TEST_CASE("Entity general behavior", "[Entity]")
{
    entity::Breed monsterBreed = entity::Breed{entity::BreedInfo{
        "Troll", "An ugly shit", 3, 100, Energy::NormalSpeed, 1, 3, Motilities{Motility::Walk}, 5},
        GraphicsInfo{sf::Texture{}}};
    entity::Monster troll{sf::Vector2i{0, 1}, monsterBreed};

    entity::Race race{{}, {}, {}};
    entity::HeroInfo info{"Dwarf", race};
    entity::Hero dwarf{sf::Vector2i{0, 1}, info};

    SECTION("An entity is dead when health reaches zero")
    {
        REQUIRE(troll.isAlive());
        troll.decreaseHealth(troll.health());
        REQUIRE(troll.health() == 0);
        REQUIRE(!troll.isAlive());

        REQUIRE(dwarf.isAlive());
        dwarf.decreaseHealth(dwarf.health());
        REQUIRE(dwarf.health() == 0);
        REQUIRE(!dwarf.isAlive());
    }

    SECTION("An entity health cannot be negative")
    {
        REQUIRE(troll.health() >= 0);
        troll.decreaseHealth(troll.health() * 2);
        REQUIRE(troll.health() >= 0);

        REQUIRE(dwarf.health() >= 0);
        dwarf.decreaseHealth(dwarf.maxHealth() * 2);
        REQUIRE(dwarf.health() >= 0);
    }

    SECTION("An entity health cannot be larger than maxHealth")
    {
        REQUIRE(troll.health() >= 0);
        troll.increaseHealth(troll.health() * 2);
        REQUIRE(troll.health() == troll.maxHealth());

        REQUIRE(dwarf.health() >= 0);
        dwarf.increaseHealth(dwarf.health() * 2);
        REQUIRE(dwarf.health() == dwarf.maxHealth());
    }

    SECTION("Each entity identifier is unique")
    {
        REQUIRE(troll.id() != dwarf.id());

        SECTION("Each copy is unique")
        {
            // Copy constructor
            const entity::Monster anotherMonster(troll); // NOLINT
            const entity::Hero anotherHero(dwarf);       // NOLINT
            REQUIRE(troll.id() != anotherMonster.id());
            REQUIRE(dwarf.id() != anotherHero.id());
        }

        SECTION("Each copy assignation is unique")
        {
            // Copy by assignation
            const entity::Monster anotherMonster = troll; // NOLINT
            const entity::Hero anotherHero = dwarf;       // NOLINT
            REQUIRE(troll.id() != anotherMonster.id());
            REQUIRE(dwarf.id() != anotherHero.id());
        }
    }
}

TEST_CASE("Monster is polymorphic", "[Entity]")
{
    entity::Breed monsterBreed = entity::Breed{entity::BreedInfo{
        "Troll", "An ugly shit", 3, 100, Energy::NormalSpeed, 1, 3, Motilities{Motility::Walk}, 5},
        GraphicsInfo{sf::Texture{}}};
    entity::Entity* troll = new entity::Monster{sf::Vector2i{0, 1}, monsterBreed};

    SECTION("Dynamic cast to child type is allowed")
    {
        CHECK(troll != nullptr);

        const entity::Monster* trollPtr = dynamic_cast<entity::Monster*>(troll);

        REQUIRE(trollPtr != nullptr);
    }

    SECTION("Type slicing does not occurs on parent copy")
    {
        CHECK(troll != nullptr);

        // Copy requires the concrete type explicitly because no clone is implemented
        entity::Entity* anotherTroll = new entity::Monster{*dynamic_cast<entity::Monster*>(troll)};
        REQUIRE(anotherTroll != nullptr);
        REQUIRE(anotherTroll != troll);

        // Polymorphic copy should call child copy constructor
        const entity::Monster* trollPtr = dynamic_cast<entity::Monster*>(troll);
        const entity::Monster* anotherTrollPtr = dynamic_cast<entity::Monster*>(anotherTroll);
        REQUIRE(trollPtr != nullptr);
        REQUIRE(anotherTrollPtr != nullptr);
        REQUIRE(trollPtr != anotherTrollPtr);

        SECTION("Copy also create a deep copy of internal MonsterState")
        {
            const entity::MonsterState* trollState = trollPtr->activeState();
            const entity::MonsterState* anotherTrollState = anotherTrollPtr->activeState();
            REQUIRE(trollState != nullptr);
            REQUIRE(trollState != anotherTrollState);
        }

        delete anotherTroll;
        REQUIRE(troll != nullptr);
    }

    delete troll;
}

TEST_CASE("Hero is polymorphic", "[Entity]")
{
    entity::Race race{{}, {}, {}};
    entity::HeroInfo info{"Dwarf", race};
    entity::Entity* dwarf = new entity::Hero{sf::Vector2i{22, 1}, info};

    SECTION("Dynamic cast to child type is allowed")
    {
        CHECK(dwarf != nullptr);

        const entity::Hero* dwarfPtr = dynamic_cast<entity::Hero*>(dwarf);
        REQUIRE(dwarfPtr != nullptr);
    }

    SECTION("Type slicing does not occurs on parent copy")
    {
        CHECK(dwarf != nullptr);
        // Copy requires the concrete type explicitly because no clone is implemented
        entity::Entity* anotherDwarf = new entity::Hero{*dynamic_cast<entity::Hero*>(dwarf)};
        REQUIRE(anotherDwarf != nullptr);
        REQUIRE(anotherDwarf != dwarf);

        // Polymorphic copy should call child copy constructor
        const entity::Hero* dwarfPtr = dynamic_cast<entity::Hero*>(dwarf);
        const entity::Hero* anotherDwarfPtr = dynamic_cast<entity::Hero*>(anotherDwarf);
        REQUIRE(dwarfPtr != nullptr);
        REQUIRE(anotherDwarfPtr != nullptr);
        REQUIRE(dwarfPtr != anotherDwarfPtr);

        SECTION("Copy also create a deep copy of internal HeroBehavior")
        {
            const entity::HeroBehavior* dwarfBehavior = dwarfPtr->activeBehavior();
            const entity::HeroBehavior* anotherDwarfBehavior = anotherDwarfPtr->activeBehavior();
            REQUIRE(dwarfBehavior != nullptr);
            REQUIRE(anotherDwarfBehavior != nullptr);
            REQUIRE(dwarfBehavior != anotherDwarfBehavior);
        }

        delete anotherDwarf;
        REQUIRE(dwarf != nullptr);
    }

    delete dwarf;
}

} // namespace cppRogue
