#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <iterator>
#include <numeric>
#include <tuple>
#include <vector>


namespace geo
{
using coord_t = double;

struct Point
{
   coord_t x;
   coord_t y;
};

class Rectangle
{
 public:
   Rectangle(Point top_left, Point bottom_right)
       : _top_left {std::move(top_left)}
       , _bottom_right {std::move(bottom_right)}
   {
      assert(
          std::tie(top_left.x, top_left.y)
          <= std::tie(bottom_right.x, bottom_right.y));
   }

   const Point& topLeft() const { return _top_left; }
   const Point& bottomRight() const { return _bottom_right; }

 private:
   Point _top_left;
   Point _bottom_right;
};

inline Rectangle MBR(const Rectangle& rect) { return rect; }

inline Rectangle MBR(const std::vector<Point>& points)
{
   auto comp_x = [](const Point& lhs, const Point& rhs)
   {
      return lhs.x < rhs.x;
   };
   auto comp_y = [](const Point& lhs, const Point& rhs)
   {
      return lhs.y < rhs.y;
   };
   auto [minx_point, maxx_point] = std::ranges::minmax_element(points, comp_x);
   auto [miny_point, maxy_point] = std::ranges::minmax_element(points, comp_y);
   return Rectangle {
       {minx_point->x, miny_point->y},
       {maxx_point->x, maxy_point->y}};
}

struct Line
{
   Point start;
   Point end;
};

inline Rectangle MBR(const Line& line)
{
   return MBR(std::vector {line.start, line.end});
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