#include "point.h"
#include <algorithm>
#include <cassert>
#include <compare>
#include <initializer_list>
#include <iterator>
#include <numeric>
#include <tuple>
#include <vector>

#ifndef RECTANGLE_H
#define RECTANGLE_H
namespace gis
{

class Rectangle
{
 public:
   Rectangle(Point top_left, Point bottom_right)
       : _top_left {std::min(top_left.x, bottom_right.x), std::max(top_left.y, bottom_right.y)}
       , _bottom_right {std::max(top_left.x, bottom_right.x), std::min(top_left.y, bottom_right.y)}
   {}

   const Point& topLeft() const { return _top_left; }
   Point        bottomLeft() const { return {_top_left.x, _bottom_right.y}; }
   Point        topRight() const { return {_bottom_right.x, _top_left.y}; }
   const Point& bottomRight() const { return _bottom_right; }

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

inline bool contains(const Rectangle& rect, const Point& p)
{
   return (not lex_comp_less(p, rect.bottomLeft()))
      and (not lex_comp_less(rect.topRight(), p));
}

inline bool intersects(const Rectangle& lhs, const Rectangle& rhs)
{
   return contains(lhs, rhs.topLeft()) or contains(lhs, rhs.bottomRight());
}
} // namespace gis
#endif