#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_RUNNER

#include "gis-utils/line_segment.h"
#include "gis-utils/rectangle.h"
#include <catch.hpp>

using namespace gis;
TEST_CASE("MBR tests")
{
   SECTION("The MBR of a rectangle is identical to the rectangle")
   {
      using p = std::pair<Point, Point>;
      auto [top_left, bottom_right] = GENERATE(
          p {Point {1, 2}, Point {2, 3}},
          p {Point {3, 4}, Point {4.32, 64.25}});
      gis::Rectangle rect {top_left, bottom_right};

      CHECK(MBR(rect) == rect);
   }
   SECTION("The MBR of a line is a rectangle to which the line is a diagonal")
   {
      using p = std::pair<LineSegment, gis::Rectangle>;
      auto [line, rect] = GENERATE(
          p {{{0, 2}, {2, 3}}, {{0, 2}, {2, 3}}},
          p {{{3, 4}, {4, 0.234}}, {{3, 0.234}, {4, 4}}},
          p {{{3, 4}, {2, 1}}, {{2, 1}, {3, 4}}},
          p {{{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}});

      CHECK(MBR(line) == rect);
   }
}