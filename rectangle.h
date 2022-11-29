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
namespace geo
{
using coord_t = double;

struct Point
{
   coord_t x;
   coord_t y;
};

inline bool operator==(const Point& lhs, const Point& rhs)
{
   return lhs.x == rhs.x and lhs.y == rhs.y;
}

bool lex_comp_less(const Point& lhs, const Point& rhs)
{
   return std::tie(lhs.x, lhs.y) < std::tie(rhs.x, rhs.y);
};

class Rectangle
{
 public:
   Rectangle(Point top_left, Point bottom_right)
       : _top_left {std::move(top_left)}
       , _bottom_right {std::move(bottom_right)}
   {
      assert(lex_comp_less(top_left, bottom_right));
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

inline bool constains(const Rectangle& rect, const Point& p)
{
   return lex_comp_less(rect.topLeft(), p)
      and lex_comp_less(p, rect.bottomRight());
}
} // namespace geo
#endif