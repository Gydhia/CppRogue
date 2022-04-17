#include <catch2/catch.hpp>

#include "validation/TestsTester.hpp"

TEST_CASE("2: Factorial of 0 is 1", "[factorial]") { REQUIRE(factorial(0) == 1); }

TEST_CASE("2: Factorials of 1 and higher can be computed", "[factorial]")
{
    REQUIRE(factorial(1) == 1);
    REQUIRE(factorial(2) == 2);

    SECTION("Section can be nested")
    {
        REQUIRE(factorial(3) == 6);
        REQUIRE(factorial(10) == 3628800);
    }
}
