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

bool lex_comp_less(const Point& lhs, const Point& rhs)
{
   return std::tie(lhs.x, lhs.y) < std::tie(rhs.x, rhs.y);
};
} // namespace geo
#endif