#include "catch2/catch.hpp"

#include "engine/core/utility/Bitflags.hpp"

/*
 Unit-Tests for Bitflags.
*/

enum class Fruit
{
    Orange,
    Apple,
    Banana,
    Coconut,
};

enum class Vegetable
{
    Carrot,
    Bean,
    Cucumber
};

using Fruits = cppRogue::Bitflags<Fruit, 4>;
using Vegetables = cppRogue::Bitflags<Vegetable, 3>;

TEST_CASE("Tests Bitflags manipulation", "[Bitflags]")
{
    auto fruits = Fruits{};
    auto vegetables = Vegetables{};

    SECTION("Bitflags start empty")
    {
        REQUIRE(fruits.disabled(Fruit::Banana));
        REQUIRE(fruits.disabled(Fruit::Orange));
        REQUIRE(fruits.disabled(Fruit::Apple));
        REQUIRE(fruits.disabled(Fruit::Coconut));
        REQUIRE(fruits.none() == true);
        REQUIRE(fruits.all() == false);
        REQUIRE(fruits.any() == false);

        REQUIRE(vegetables.disabled(Vegetable::Carrot));
        REQUIRE(vegetables.disabled(Vegetable::Bean));
        REQUIRE(vegetables.disabled(Vegetable::Cucumber));
        REQUIRE(vegetables.none() == true);
        REQUIRE(vegetables.all() == false);
        REQUIRE(vegetables.any() == false);
    }

    SECTION("Bitflags can be compared")
    {
        auto fruits = Fruits{};
        auto fruits2 = fruits;
        REQUIRE(fruits == fruits);
        REQUIRE(fruits2 == fruits2);
        REQUIRE(fruits == fruits2);
        REQUIRE(fruits2 == fruits);
    }

    SECTION("Can set flags")
    {
        REQUIRE(fruits.none() == true);

        fruits.set(Fruit::Banana);
        REQUIRE(fruits.enabled(Fruit::Banana));

        fruits.set(Fruit::Orange);
        REQUIRE(fruits.enabled(Fruit::Banana));
        REQUIRE(fruits.enabled(Fruit::Orange));

        fruits.set(Fruit::Apple);
        REQUIRE(fruits.enabled(Fruit::Banana));
        REQUIRE(fruits.enabled(Fruit::Orange));
        REQUIRE(fruits.enabled(Fruit::Apple));

        fruits.set(Fruit::Coconut);
        REQUIRE(fruits.enabled(Fruit::Banana));
        REQUIRE(fruits.enabled(Fruit::Orange));
        REQUIRE(fruits.enabled(Fruit::Apple));
        REQUIRE(fruits.enabled(Fruit::Coconut));

        REQUIRE(fruits.all() == true);

        SECTION("Can unset flags")
        {
            REQUIRE(fruits.all() == true);

            fruits.unset(Fruit::Banana);
            REQUIRE(fruits.disabled(Fruit::Banana));

            fruits.unset(Fruit::Orange);
            REQUIRE(fruits.disabled(Fruit::Orange));

            fruits.unset(Fruit::Apple);
            REQUIRE(fruits.disabled(Fruit::Apple));

            fruits.unset(Fruit::Coconut);
            REQUIRE(fruits.disabled(Fruit::Coconut));

            REQUIRE(fruits.none() == true);
        }
    }

    SECTION("Can combine flags")
    {
        fruits.set(Fruit::Banana);

        auto justApple = Fruits{Fruit::Apple};

        // Using + operator
        auto both = fruits + justApple;
        REQUIRE(fruits.overlaps(justApple) == false);
        REQUIRE(fruits.overlaps(both) == true);
        REQUIRE(justApple.overlaps(both) == true);
        REQUIRE(both.enabled(Fruit::Apple) == true);
        REQUIRE(both.enabled(Fruit::Banana) == true);

        // Using | operator
        auto both2 = fruits | justApple;
        REQUIRE(fruits.overlaps(justApple) == false);
        REQUIRE(fruits.overlaps(both2) == true);
        REQUIRE(justApple.overlaps(both2) == true);
        REQUIRE(both2.enabled(Fruit::Apple) == true);
        REQUIRE(both2.enabled(Fruit::Banana) == true);

        REQUIRE(both == both2);
        REQUIRE(both.overlaps(both2));
    }

    SECTION("Can substract flags")
    {
        fruits.set(Fruit::Banana);
        fruits.set(Fruit::Apple);
        REQUIRE(fruits.enabled(Fruit::Banana));
        REQUIRE(fruits.enabled(Fruit::Apple));

        auto justApple = Fruits{Fruit::Apple};

        // Using - operator
        auto onlyBanana = fruits;
        onlyBanana -= Fruit::Apple;
        REQUIRE(onlyBanana.enabled(Fruit::Banana));
        REQUIRE(onlyBanana.disabled(Fruit::Apple));
    }

    SECTION("Count return the number of active enums")
    {
        REQUIRE(fruits.none() == true);
        REQUIRE(fruits.count() == 0);

        fruits += Fruit::Apple;
        fruits |= Fruit::Apple;

        REQUIRE(fruits.any() == true);
        REQUIRE(fruits.none() == false);
        REQUIRE(fruits.count() == 1);

        fruits |= Fruit::Banana;
        REQUIRE(fruits.count() == 2);

        fruits += Fruit::Coconut;
        REQUIRE(fruits.count() == 3);

        fruits |= Fruit::Orange;
        REQUIRE(fruits.count() == 4);

        SECTION("Removing flag does not invalidate the count")
        {
            REQUIRE(fruits.count() == 4);

            fruits -= Fruit::Banana;
            REQUIRE(fruits.count() == 3);

            fruits -= Fruit::Coconut;
            REQUIRE(fruits.count() == 2);

            fruits -= Fruit::Orange;
            REQUIRE(fruits.count() == 1);

            fruits -= Fruit::Apple;
            REQUIRE(fruits.count() == 0);
            REQUIRE(fruits.none() == true);
            REQUIRE(fruits.any() == false);
        }
    }
}
