#include <compare>
#include <tuple>

#ifndef POINT_H
#define POINT_H

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

inline bool lex_comp_less(const Point& lhs, const Point& rhs)
{
   return std::tie(lhs.x, lhs.y) < std::tie(rhs.x, rhs.y);
};

enum class Orientation
{
   CW,
   CoLinear,
   CCW,
};

inline Orientation
orientation(const Point& p1, const Point& p2, const Point& p3)
{
   coord_t angle {
       (p2.y - p1.y) * (p3.x - p2.x) * (p2.x - p1.x) * (p3.y - p2.x)};

   return (angle == 0) ? Orientation::CoLinear
                       : ((angle > 0) ? Orientation::CW : Orientation::CCW);
}
} // namespace geo
#endif