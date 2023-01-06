#ifndef GEOMETRY_H
#define GEOMETRY_H
#include "line_segment.h"
#include "rectangle.h"
#include <variant>

namespace gis
{
using Geometry = std::variant<
    Rectangle,
    LineSegment,
    PolyLine<LineType::Open>,
    PolyLine<LineType::Closed>,
    Polygon>;

inline Rectangle MBR(const Geometry& geometry)
{
   return std::visit([](auto&& inner) { return MBR(inner); }, geometry);
}

inline std::vector<LineSegment> to_segments(const Geometry& geometry)
{
   return std::visit([](auto&& inner) { return to_segments(inner); }, geometry);
}

inline bool intersects(const Geometry& lhs, const Geometry& rhs)
{
   if (not intersects(MBR(lhs), MBR(rhs)))
      return false;
   auto lhs_segments {to_segments(lhs)};
   auto rhs_segments {to_segments(rhs)};
   lhs_segments.reserve(rhs_segments.size());
   std::move(
       std::begin(rhs_segments),
       std::end(rhs_segments),
       std::back_inserter(lhs_segments));
   return exists_intersection(lhs_segments);
}
} // namespace gis
#endif