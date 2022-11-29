#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_RUNNER

#include "line.h"
#include "rectangle.h"
#include <catch.hpp>

using namespace geo;
TEST_CASE("MBR tests")
{
   SECTION("The MBR of a rectangle is identical to the rectangle")
   {
      using p = std::pair<Point, Point>;
      auto [top_left, bottom_right] = GENERATE(
          p {Point {1, 2}, Point {2, 3}},
          p {Point {3, 4}, Point {4.32, 64.25}});
      Rectangle rect {top_left, bottom_right};

      CHECK(MBR(rect) == rect);
   }
}