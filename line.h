#include "rectangle.h"
#include <optional>

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

struct LineCoefficients
{
   LineCoefficients(const Line& line)
       : a {-(line.end.y - line.start.y) / (line.end.x - line.start.x)}
       , c {a * line.start.y}
   {}

   coord_t a {};
   coord_t c {};
};

std::optional<Point> intersection(const Line& l1, const Line& l2)
{
   LineCoefficients lc1 {l1};
   LineCoefficients lc2 {l2};

   if (lc1.a == lc2.a)
      return std::nullopt;

   Point intersection {
       (lc2.c - lc1.c) / (lc1.a - lc2.a),
       (lc1.c * lc2.a - lc2.c * lc1.a) / (lc1.a - lc2.a)};
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