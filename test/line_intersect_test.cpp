#include "point.h"
#include <catch.hpp>
#include <tuple>

using namespace geo;
TEST_CASE("Orientation")
{
   using t = std::tuple<Point, Point, Point>;
   SECTION("Colinear points")
   {
      auto [p1, p2, p3] = GENERATE(
          t {{0, 0}, {0, 0}, {0, 0}},
          t {{1, 1}, {2, 2}, {3, 3}},
          t {{0, 0}, {0.5, -0.5}, {1, -1}});

      CHECK(orientation(p1, p2, p3) == Orientation::CoLinear);
   }
   SECTION("clockwise oriented points")
   {
      auto [p1, p2, p3] = GENERATE(
          t {{0, 0}, {1, 0}, {2, -1}},
          t {{1, 3}, {2, 2}, {1, 1}},
          t {{0, 0}, {0.5, -0.5}, {0.5, -1}});

      CHECK(orientation(p1, p2, p3) == Orientation::CW);
   }
   SECTION("counter-clockwise oriented points")
   {
      auto [p1, p2, p3] = GENERATE(
          t {{0, 0}, {1, 0}, {2, 1}},
          t {{1, 1}, {2, 2}, {1, 3}},
          t {{0.5, -1}, {0.5, -0.5}, {0, 0}});

      CHECK(orientation(p1, p2, p3) == Orientation::CCW);
   }
}