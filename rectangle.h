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
       : _top_left {std::move(top_left)}
       , _bottom_right {std::move(bottom_right)}
   {
      assert(not lex_comp_less(bottom_right, top_left));
   }

   const Point& topLeft() const { return _top_left; }
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
   return (lex_comp_less(rect.topLeft(), p) or p == rect.topLeft())
      and (lex_comp_less(p, rect.bottomRight()) or p == rect.bottomRight());
}

inline bool intersects(const Rectangle& lhs, const Rectangle& rhs)
{
   return contains(lhs, rhs.topLeft()) or contains(lhs, rhs.bottomRight());
}
} // namespace gis
#endif