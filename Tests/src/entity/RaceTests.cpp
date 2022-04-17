#include <catch2/catch.hpp>
#include <cstddef>

#include "engine/entity/hero/Race.hpp"

/*
 Unit-Tests for Race.
*/

namespace cppRogue {

TEST_CASE("Race creation", "[Race]")
{
    SECTION("Can create a default Race")
    {
        entity::Race race{{}, {}, {}};

        REQUIRE(race.description().empty());
        REQUIRE(race.name().empty());

        REQUIRE(race.property(entity::Statistic::Strength) == 1);
        REQUIRE(race.property(entity::Statistic::Endurance) == 1);
        REQUIRE(race.property(entity::Statistic::Agility) == 1);
        REQUIRE(race.property(entity::Statistic::Intellect) == 1);
    }

    SECTION("Can create a Race with user define fields")
    {
        const std::string name{"Dwarf"};
        const std::string description{
            "Dwarf are small but stocky humanoid stone creature almost always bearded"};
        entity::Race::StatisticsMap stats{};
        stats[entity::Statistic::Strength] = 13;
        stats[entity::Statistic::Endurance] = 2;

        entity::Race race{name, description, stats};

        REQUIRE(race.description() == description);
        REQUIRE(race.name() == name);
        REQUIRE(race.property(entity::Statistic::Strength) == stats[entity::Statistic::Strength]);
        REQUIRE(race.property(entity::Statistic::Endurance) == stats[entity::Statistic::Endurance]);
        REQUIRE(race.property(entity::Statistic::Agility) == 1);
        REQUIRE(race.property(entity::Statistic::Intellect) == 1);
    }
}

} // namespace cppRogue
