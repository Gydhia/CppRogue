#include "catch2/catch.hpp"

#include "engine/core/Energy.hpp"

/*
 Unit-Tests for Energy.
*/

namespace cppRogue {

TEST_CASE("Can create an Energy", "[Energy]")
{
    auto energy = Energy{};

    SECTION("Energy default values are valid")
    {
        REQUIRE(energy.ActionThreshold == 250);
        REQUIRE(energy.MinSpeed == 0);
        REQUIRE(energy.NormalSpeed == 5);
        REQUIRE(energy.ReferenceGain == 60);
        REQUIRE(energy.MaxSpeed == 10);
    }

    SECTION("Energy amount at initialization is 0") { REQUIRE(energy.amount() == 0); }
}

TEST_CASE("Energy manipulation", "[Energy]")
{
    auto energy = Energy{};

    SECTION("Accumulate increase energy amount")
    {
        int currentAmount = energy.amount();
        REQUIRE(currentAmount == 0);

        int addedAmount = 1;
        currentAmount += Energy::SpeedTable[addedAmount];
        energy.accumulate(addedAmount);
        REQUIRE(energy.amount() == currentAmount);

        addedAmount = 10;
        currentAmount += Energy::SpeedTable[addedAmount];
        energy.accumulate(addedAmount);
        REQUIRE(energy.amount() == currentAmount);
    }

    SECTION("Taking a turn requires to accumulate enough energy")
    {
        const auto threshold = Energy::ActionThreshold;
        REQUIRE(energy.amount() == 0);
        REQUIRE(energy.amount() < threshold);
        REQUIRE(energy.canTakeTurn() == false);

        // Not enough energy
        energy.accumulate(10);
        REQUIRE(energy.amount() > 0);
        REQUIRE(energy.amount() < threshold);
        REQUIRE(energy.canTakeTurn() == false);

        // Enough energy
        energy.accumulate(2);
        REQUIRE(energy.amount() > threshold);
        REQUIRE(energy.canTakeTurn() == true);

        SECTION("Spending energy decreases the ability to take a turn")
        {
            REQUIRE(energy.canTakeTurn() == true);
            energy.spend();
            REQUIRE(energy.canTakeTurn() == false);
        }
    }
}

} // namespace cppRogue