#include <cassert>
#include <cmath>
#include <compare>
#include <tuple>
#include <type_traits>

#ifndef POINT_H
#define POINT_H

namespace gis
{
using coord_t = double;

template <typename T>
concept Arithmetic = std::integral<T> or std::floating_point<T>;

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

inline Point operator+(const Point& lhs, const Point& rhs)
{
   return {lhs.x + rhs.x, lhs.y + rhs.y};
}

inline Point operator-(const Point& lhs, const Point& rhs)
{
   return {lhs.x - rhs.x, lhs.y - rhs.y};
}

inline Point operator*(const Point& p, Arithmetic auto scalar)
{
   return {p.x * scalar, p.y * scalar};
}
inline Point operator*(Arithmetic auto scalar, const Point& p)
{
   return p * scalar;
}

inline Point operator/(const Point& p, Arithmetic auto scalar)
{
   assert(scalar != 0);
   return {p.x / scalar, p.y / scalar};
}

inline coord_t dot_product(const Point& lhs, const Point& rhs)
{
   return lhs.x * rhs.x + lhs.y * rhs.y;
}

inline coord_t distance_squared(const Point& p1, const Point& p2)
{
   return dot_product(p1 - p2, p1 - p2);
}

inline coord_t distance(const Point& p1, const Point& p2)
{
   return std::sqrt(distance_squared(p1, p2));
}

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
       (p2.y - p1.y) * (p3.x - p2.x) - (p2.x - p1.x) * (p3.y - p2.y)};

   return (angle == 0) ? Orientation::CoLinear
                       : ((angle > 0) ? Orientation::CW : Orientation::CCW);
}
} // namespace gis
#endif