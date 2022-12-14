#include "rectangle.h"
#include <optional>

#ifndef LINE_H
#define LINE_H
namespace geo
{

struct Line
{
   Line(Point start, Point end)
       : start {lex_comp_less(start, end) ? std::move(start) : std::move(end)}
       , end {lex_comp_less(start, end) ? std::move(end) : std::move(start)}
   {}

   Point start;
   Point end;
};

inline Rectangle MBR(const Line& line)
{
   return MBR(std::vector {line.start, line.end});
}

bool intersects(const Line& l1, const Line& l2);

bool exists_intersection(const std::vector<Line>& lines);

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
std::vector<Line> to_lines(const PolyLine<type> poly_line)
{
   std::vector<Line> res {};
   std::transform(
       std::begin(poly_line.points()),
       std::end(poly_line.points()),
       std::begin(poly_line.points()) + 1,
       std::back_inserter(res),
       [](const Point& start, const Point& end) {
          return Line {start, end};
       });
   return res;
}

using Polygon = PolyLine<LineType::Filled>;

template <LineType type> inline Rectangle MBR(const PolyLine<type>& line)
{
   return MBR(line.points());
}

inline bool intersects(const Polygon& lhs, const Polygon& rhs)
{
   if (not intersects(MBR(lhs), MBR(rhs)))
      return false;

   auto lhs_segments {to_lines(lhs)};
   auto rhs_segments {to_lines(rhs)};
   std::move(
       std::begin(rhs_segments),
       std::end(rhs_segments),
       std::back_inserter(lhs_segments));
   return exists_intersection(lhs_segments);
}
} // namespace geo
#endif