#include "rectangle.h"
#include <optional>

#ifndef LINE_H
#define LINE_H
namespace gis
{

struct LineSegment
{
   LineSegment(Point start, Point end)
       : start {std::move(start)}
       , end {std::move(end)}
   {}

   Point start;
   Point end;
};

coord_t min_distance(const LineSegment& s, const Point& p);

inline Rectangle MBR(const LineSegment& segment)
{
   return MBR(std::vector {segment.start, segment.end});
}

bool intersects(const LineSegment& l1, const LineSegment& l2);

inline std::vector<LineSegment> to_segments(const LineSegment& segment)
{
   return {segment};
}

bool exists_intersection(const std::vector<LineSegment>& lines);

enum class LineType
{
   Open,
   Closed,
   Filled
};

template <LineType type> class PolyLine
{
 public:
   PolyLine(std::initializer_list<Point> points)
       : _points {std::move(points)}
   {}

   const std::vector<Point>& points() const { return _points; }

 private:
   std::vector<Point> _points;
};

template <LineType type>
std::vector<LineSegment> to_segments(const PolyLine<type> poly_line)
{
   if (poly_line.points().empty())
      return {};
   if (std::ssize(poly_line.points()) < 2)
      return {{poly_line.points().front(), poly_line.points().front()}};
   if (std::ssize(poly_line.points()) == 2)
      return {{poly_line.points().front(), poly_line.points().back()}};

   auto                     n_segments {std::ssize(poly_line.points()) - 1};
   std::vector<LineSegment> res {};
   res.reserve(std::max(n_segments, 0ll));
   std::transform(
       std::begin(poly_line.points()),
       std::end(poly_line.points()) - 1,
       std::begin(poly_line.points()) + 1,
       std::back_inserter(res),
       [](const Point& start, const Point& end) {
          return LineSegment {start, end};
       });
   res.push_back({poly_line.points().back(), poly_line.points().front()});
   return res;
}

template <>
inline std::vector<LineSegment>
to_segments<LineType::Open>(const PolyLine<LineType::Open> poly_line)
{
   if (poly_line.points().empty())
      return {};
   if (std::ssize(poly_line.points()) < 2)
      return {{poly_line.points().front(), poly_line.points().front()}};
   if (std::ssize(poly_line.points()) == 2)
      return {{poly_line.points().front(), poly_line.points().back()}};

   auto                     n_segments {std::ssize(poly_line.points()) - 1};
   std::vector<LineSegment> res {};
   res.reserve(std::max(n_segments, 0ll));
   std::transform(
       std::begin(poly_line.points()),
       std::end(poly_line.points()),
       std::begin(poly_line.points()) + 1,
       std::back_inserter(res),
       [](const Point& start, const Point& end) {
          return LineSegment {start, end};
       });
   return res;
}

using Polygon = PolyLine<LineType::Filled>;

template <LineType type> inline Rectangle MBR(const PolyLine<type>& line)
{
   return MBR(line.points());
}

template <LineType t1, LineType t2>
inline bool intersects(const PolyLine<t1>& lhs, const PolyLine<t2>& rhs)
{
   if (not intersects(MBR(lhs), MBR(rhs)))
      return false;

   auto lhs_segments {to_segments(lhs)};
   auto rhs_segments {to_segments(rhs)};
   std::move(
       std::begin(rhs_segments),
       std::end(rhs_segments),
       std::back_inserter(lhs_segments));
   return exists_intersection(lhs_segments);
}

template <LineType type>
bool contains(const PolyLine<type>& poly_line, const Point& point)
{
   auto crosses_upwards_on_y =
       [](const LineSegment& segment, const Point& point)
   {
      return segment.start.y <= point.y and segment.end.y > point.y;
   };
   auto crosses_downwards_on_y =
       [](const LineSegment& segment, const Point& point)
   {
      return segment.end.y <= point.y and segment.start.y > point.y;
   };

   int64_t winding_count {0};
   for (const auto& segment : to_segments(poly_line))
   {
      winding_count +=
          (crosses_upwards_on_y(segment, point)
           and (orientation(segment.start, segment.end, point) == Orientation::CCW))
          - (crosses_downwards_on_y(segment, point)
             and (orientation(segment.start, segment.end, point) == Orientation::CW));
   }
   return winding_count != 0;
}

template <>
inline bool contains<LineType::Open>(
    const PolyLine<LineType::Open>& poly_line,
    const Point&                    point)
{
   return false;
}
} // namespace gis
#endif