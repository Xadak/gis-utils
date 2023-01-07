#include "gis-utils/geometry.h"
#include <catch.hpp>

TEST_CASE("contains")
{
   SECTION("Polygon and point")
   {
      SECTION("Point inside polygon")
      {
         gis::Point point {1, 1};
         auto       polygon = GENERATE(
             gis::Polygon {{0, 0}, {2, 0}, {1, 2}},
             gis::Polygon {{0, 0}, {2, 0}, {2, 1}, {1.5, 1}, {1.5, 2}, {0, 2}});

         CHECK(gis::contains(polygon, point));
      }
      SECTION("Point outside polygon")
      {
         gis::Point point {1, 1};
         auto       polygon = GENERATE(
             gis::Polygon {{0, 0}, {2, 0}, {1, 0.5}},
             gis::Polygon {
                       {0, 0},
                       {2, 0},
                       {2, 2},
                       {0, 2},
                       {0, 1.5},
                       {1.5, 1.5},
                       {1.5, 0.5},
                       {0.5, 0.5},
                       {0.5, 1.5},
                       {0, 1.5}});

         CHECK_FALSE(gis::contains(polygon, point));
      }
      SECTION(
          "Points on the lower left edges of polygons are considered contained")
      {
         gis::Point point {1, 1};
         auto       polygon = GENERATE(
             gis::Polygon {{0, 1}, {2, 1}, {1, 1.5}},
             gis::Polygon {{0, 0}, {0, 2}, {2, 2}, {3, 0}});

         CHECK(gis::contains(polygon, point));
      }
      SECTION("Points on the upper right edges of polygons are considered not "
              "contained")
      {
         gis::Point point {1, 1};
         auto       polygon = GENERATE(
             gis::Polygon {{0, 0}, {2, 0}, {1, 1}},
             gis::Polygon {{0, 0}, {1, 0}, {1, 1}, {0, 1}});

         CHECK_FALSE(gis::contains(polygon, point));
      }
   }
}