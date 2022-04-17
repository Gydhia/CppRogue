#include <catch2/catch.hpp>
#include <cstddef>

#include "engine/entity/hero/Property.hpp"
#include "engine/entity/hero/Statistic.hpp"

/*
 Unit-Tests for Property.
*/

namespace cppRogue {

TEST_CASE("Property creation", "[Property]")
{
    entity::Property property{entity::Statistic::Agility, "Agility", 21};

    SECTION("Fields are correctly assigned")
    {
        REQUIRE(property.kind() == entity::Statistic::Agility);
        REQUIRE(property.name() == "Agility");

        SECTION("Minimal and initial value is 1") { REQUIRE(property.value() == 1); }
    }
}

TEST_CASE("Value can be updated by updating internal fields", "[Property]")
{
    entity::Property property{entity::Statistic::Intellect, "Intellect", 21};
    property.set(10);
    property.setBonus(2);
    property.setOffset(4);

    REQUIRE(property.value() == 16);

    SECTION("Value can never be larger than max value")
    {
        property.setBonus(200);

        REQUIRE(property.value() == property.maxValue());
    }

    SECTION("Value can never be smaller than 1")
    {
        property.setBonus(-200);

        REQUIRE(property.value() == 1);
    }
}

} // namespace cppRogue
