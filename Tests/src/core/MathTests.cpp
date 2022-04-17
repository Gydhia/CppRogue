#include "catch2/catch.hpp"

#include "engine/core/utility/Math.hpp"

/*
 Unit-Tests for Math functions.
*/

namespace cppRogue {

TEST_CASE("lerp function", "[Math]")
{
    REQUIRE(math::lerp(1, 0, 2, 10.0, 20.0) == 15);
    REQUIRE(math::lerp(1, 0, 5, 10.0, 20.0) == 12);
    REQUIRE(math::lerp(2, 0, 5, 10.0, 20.0) == 14);
    REQUIRE(math::lerp(2, 0, 5, 10.0, 11.0) == 10.4);
}

TEST_CASE("chebyshevDistance function", "[Math]")
{
    REQUIRE(math::chebyshevDistance({2, 4}, {1, 2}) == 2);
    REQUIRE(math::chebyshevDistance({2, 8}, {-11, 2}) == 13);
    REQUIRE(math::chebyshevDistance({2, 2}, {2, 2}) == 0);
}

} // namespace cppRogue
