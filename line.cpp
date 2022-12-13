#include "line.h"

bool geo::intersects(const Line& l1, const Line& l2)
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