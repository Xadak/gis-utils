#include "rectangle.h"
#include <optional>

#ifndef LINE_H
#define LINE_H
namespace geo
{

struct Line
{
   Point start;
   Point end;
};

inline Rectangle MBR(const Line& line)
{
   return MBR(std::vector {line.start, line.end});
}

inline bool intersects(const Line& l1, const Line& l2)
{
   auto on_segment_when_colinear = [](const Line& line, const Point& point)
   {
      return contains(MBR(line), point);
   };

   Orientation o1 {orientation(l1.start, l1.end, l2.start)};
   Orientation o2 {orientation(l1.start, l1.end, l2.end)};
   Orientation o3 {orientation(l2.start, l2.end, l1.start)};
   Orientation o4 {orientation(l2.start, l2.end, l1.end)};

   if (o1 != o2 and o3 != o4)
      return true;

   if (o1 == Orientation::CoLinear and on_segment_when_colinear(l1, l2.start))
      return true;
   if (o2 == Orientation::CoLinear and on_segment_when_colinear(l1, l2.end))
      return true;
   if (o3 == Orientation::CoLinear and on_segment_when_colinear(l2, l1.start))
      return true;
   if (o4 == Orientation::CoLinear and on_segment_when_colinear(l2, l1.end))
      return true;

   return false;
}

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
} // namespace geo
#endif