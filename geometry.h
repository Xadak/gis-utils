#include <cassert>
#include <tuple>

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
       : _top_left {top_left}
       , _bottom_right {bottom_right}
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
} // namespace geo