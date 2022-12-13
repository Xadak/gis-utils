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

bool intersects(const Line& l1, const Line& l2);

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