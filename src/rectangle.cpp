#include "gis-utils/rectangle.h"
#include "gis-utils/line_segment.h"

std::vector<gis::LineSegment> gis::to_segments(const Rectangle& rect)
{
   return {
       {rect.topLeft(), rect.topRight()},
       {rect.topRight(), rect.bottomRight()},
       {rect.bottomRight(), rect.bottomLeft()},
       {rect.bottomLeft(), rect.topLeft()}};
}