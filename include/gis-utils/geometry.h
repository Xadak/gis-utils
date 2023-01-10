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

inline bool contains(const Geometry& lhs, const Geometry& rhs)
{
   auto contains_at_least_one_point = [](auto&& lhs, auto&& rhs)
   {
      return std::ranges::any_of(
          rhs.points(),
          [&](auto&& point) { return contains(lhs, point); });
   };

   if (not contains(MBR(lhs), MBR(rhs)))
      return false;
   return not intersects(lhs, rhs)
      and std::visit(contains_at_least_one_point, lhs, rhs);
}

inline bool contains(const Geometry& g, const Point& p)
{
   return std::visit(
       [&p](auto&& geometry) { return contains(geometry, p); },
       g);
}

inline coord_t min_distance(const Geometry& g, const Point& p)
{
   if (contains(g, p))
      return 0;

   return std::ranges::min(
       to_segments(g)
       | std::views::transform([&](const LineSegment& segment)
                               { return min_distance(segment, p); }));
}

inline coord_t min_distance(const Geometry& lhs, const Geometry& rhs)
{
   if (contains(lhs, rhs) or contains(rhs, lhs) or intersects(lhs, rhs)
       or lhs == rhs)
      return 0;

   auto min_dist_from_lhs_vert_to_rhs = [](const auto& lhs,
                                           const auto& rhs) -> coord_t
   {
      auto rhs_segments {to_segments(rhs)};
      auto distances_from_verts =
          lhs.points()
          | std::views::transform(
              [&](const auto& point)
              {
                 return std::ranges::min(
                     rhs_segments
                     | std::views::transform(
                         [&](const LineSegment& segment)
                         { return min_distance(segment, point); }));
              });
      return std::ranges::min(distances_from_verts);
   };

   return std::visit(
       [=](const auto& lhs, const auto& rhs) -> coord_t
       {
          return std::ranges::min(
              min_dist_from_lhs_vert_to_rhs(lhs, rhs),
              min_dist_from_lhs_vert_to_rhs(rhs, lhs));
       },
       lhs,
       rhs);
}
} // namespace gis
#endif