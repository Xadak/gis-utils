#include "point.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <compare>
#include <initializer_list>
#include <iterator>
#include <numeric>
#include <ranges>
#include <tuple>
#include <vector>

#ifndef RECTANGLE_H
#define RECTANGLE_H
namespace gis
{

class Rectangle
{
 public:
   Rectangle(Point p1, Point p2)
       : _top_left {std::min(p1.x, p2.x), std::max(p1.y, p2.y)}
       , _bottom_right {std::max(p1.x, p2.x), std::min(p1.y, p2.y)}
   {}

   const Point& topLeft() const { return _top_left; }
   Point        bottomLeft() const { return {_top_left.x, _bottom_right.y}; }
   Point        topRight() const { return {_bottom_right.x, _top_left.y}; }
   const Point& bottomRight() const { return _bottom_right; }

   std::array<Point, 4> points() const
   {
      return {topLeft(), topRight(), bottomLeft(), bottomRight()};
   }

 private:
   Point _top_left;
   Point _bottom_right;
};

inline bool operator==(const Rectangle& lhs, const Rectangle& rhs)
{
   return lhs.topLeft() == rhs.topLeft()
      and lhs.bottomRight() == rhs.bottomRight();
}

inline Rectangle MBR(const Rectangle& rect) { return rect; }

inline Rectangle MBR(const std::vector<Point>& points)
{
   auto comp_x = [](const Point& lhs, const Point& rhs)
   {
      return lhs.x < rhs.x;
   };
   auto comp_y = [](const Point& lhs, const Point& rhs)
   {
      return lhs.y < rhs.y;
   };
   auto [minx_point, maxx_point] = std::ranges::minmax_element(points, comp_x);
   auto [miny_point, maxy_point] = std::ranges::minmax_element(points, comp_y);
   return Rectangle {
       {minx_point->x, miny_point->y},
       {maxx_point->x, maxy_point->y}};
}

inline coord_t area(const Rectangle& rect)
{
   auto top_right {rect.topRight()};
   return (top_right.x - rect.topLeft().x)
        * (rect.bottomRight().y - top_right.y);
}

inline bool contains(const Rectangle& rect, const Point& p)
{
   auto down_and_or_to_the_left_of = [](const Point& p1, const Point& p2)
   {
      return p1.x < p2.x or p1.y < p2.y;
   };
   return (not down_and_or_to_the_left_of(p, rect.bottomLeft()))
      and (not down_and_or_to_the_left_of(rect.topRight(), p));
}

inline bool intersects(const Rectangle& lhs, const Rectangle& rhs)
{
   auto is_to_the_left_of = [](const Rectangle& rect, const Rectangle& other)
   {
      return rect.bottomRight().x < other.topLeft().x;
   };
   auto is_above = [](const Rectangle& rect, const Rectangle& other)
   {
      return rect.bottomRight().y > other.topLeft().y;
   };
   using p = std::pair<Rectangle, Rectangle>;
   for (const auto& [rect, other] : {p {lhs, rhs}, p {rhs, lhs}})
   {
      if (area(other) == 0)
         return false;
      if (is_to_the_left_of(rect, other))
         return false;
      if (is_above(rect, other))
         return false;
   }
   return true;
}

inline bool contains(const Rectangle& lhs, const Rectangle& rhs)
{
   return not intersects(lhs, rhs)
      and std::ranges::any_of(
          rhs.points(),
          [&](auto&& point) { return contains(lhs, point); });
}

struct LineSegment;

std::vector<LineSegment> to_segments(const Rectangle& rect);
} // namespace gis
#endif