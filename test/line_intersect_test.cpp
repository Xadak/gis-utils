#include "line.h"
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

TEST_CASE("Line segment intersection")
{
   using p = std::pair<Line, Line>;
   SECTION("non-parallel intersecting")
   {
      auto [l1, l2] = GENERATE(
          p {{{1, 1}, {3, 1}}, {{2, 0}, {2, 2}}},
          p {{{0, 0}, {2, 2}}, {{0, 2}, {2, 0}}},
          p {{{1, 1}, {3, 1}}, {{3, 1}, {4, 0}}});

      CHECK(intersects(l1, l2));
   }
   SECTION("non-parallel non-intersecting")
   {
      auto [l1, l2] = GENERATE(
          p {{{1, 2}, {3, 2}}, {{2, 0}, {2, 1}}},
          p {{{0, 0}, {2, 2}}, {{0, 5}, {2, 3}}},
          p {{{1, 1}, {3, 1}}, {{3.5, 1}, {4, 0}}});

      CHECK_FALSE(intersects(l1, l2));
   }
   SECTION("parallel intersecting")
   {
      auto [l1, l2] = GENERATE(
          p {{{1, 2}, {3, 2}}, {{2, 2}, {4, 2}}},
          p {{{0, 0}, {2, 2}}, {{1, 1}, {3, 3}}},
          p {{{0, 0}, {1, 2}}, {{1, 2}, {2, 4}}});

      CHECK(intersects(l1, l2));
   }
   SECTION("parallel non intersecting")
   {
      auto [l1, l2] = GENERATE(
          p {{{1, 2}, {2, 2}}, {{3, 2}, {4, 2}}},
          p {{{0, 0}, {1, 1}}, {{2, 2}, {3, 3}}},
          p {{{0, 0}, {1, 2}}, {{1.5, 2}, {2, 4}}});

      CHECK_FALSE(intersects(l1, l2));
   }
}

TEST_CASE("Multi-segment intersection query")
{
   SECTION("All parallel horizontal non-intersecting")
   {
      std::vector<Line> segments {
          {{0, 0}, {1, 0}},
          {{0, 1}, {2, 1}},
          {{1, 2.5}, {2, 2.5}},
      };

      CHECK_FALSE(exists_intersection(segments));
   }
   SECTION("All parallel horizontal non-intersecting")
   {
      std::vector<Line> segments {
          {{0, 0}, {0, 1}},
          {{1, 0}, {1, 2}},
          {{2.5, 1}, {2.5, 2}},
      };

      CHECK_FALSE(exists_intersection(segments));
   }
   SECTION("Two overlapping horizontal")
   {
      std::vector<Line> segments {
          {{0, 0}, {1, 0}},
          {{0.5, 0}, {2, 0}},
          {{10, 10}, {10, 12}},
      };

      CHECK(exists_intersection(segments));
   }
   SECTION("Two overlapping vertical")
   {
      std::vector<Line> segments {
          {{0, 0}, {0, 1}},
          {{0, 0.5}, {0, 2}},
          {{10, 10}, {10, 12}},
      };

      CHECK(exists_intersection(segments));
   }
   SECTION("Two intersecting on different end-points")
   {
      std::vector<Line> segments {
          {{0, 0}, {1, 0}},
          {{1, 0}, {2, 3}},
          {{10, 10}, {10, 12}},
      };

      CHECK(exists_intersection(segments));
   }
   SECTION("Two intersecting on left end-points")
   {
      std::vector<Line> segments {
          {{0, 0}, {1, 0}},
          {{0, 0}, {2, 3}},
          {{10, 10}, {10, 12}},
      };

      CHECK(exists_intersection(segments));
   }
   SECTION("Two intersecting on right end-points")
   {
      std::vector<Line> segments {
          {{0, 0}, {1, 0}},
          {{0, 3}, {1, 0}},
          {{10, 10}, {10, 12}},
      };

      CHECK(exists_intersection(segments));
   }
   SECTION("Two intersecting on non end-point")
   {
      std::vector<Line> segments {
          {{0, 0}, {1, 1}},
          {{0, 1}, {1, 0}},
          {{10, 10}, {10, 12}},
      };

      CHECK(exists_intersection(segments));
   }
   SECTION("three or more intersecting on non end-point")
   {
      std::vector<Line> segments {
          {{0, 0}, {1, 1}},
          {{0, 1}, {1, 0}},
          {{0.5, 1}, {0.5, 0}},
      };

      CHECK(exists_intersection(segments));
   }
}