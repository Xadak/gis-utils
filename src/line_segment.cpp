#include "gis-utils/line_segment.h"
#include <set>

bool gis::intersects(const LineSegment& s1, const LineSegment& s2)
{
   auto on_segment_when_colinear =
       [](const LineSegment& segment, const Point& point)
   {
      return contains(MBR(segment), point);
   };

   Orientation o1 {orientation(s1.start, s1.end, s2.start)};
   Orientation o2 {orientation(s1.start, s1.end, s2.end)};
   Orientation o3 {orientation(s2.start, s2.end, s1.start)};
   Orientation o4 {orientation(s2.start, s2.end, s1.end)};

   if (o1 != o2 and o3 != o4)
      return true;

   if (o1 == Orientation::CoLinear and on_segment_when_colinear(s1, s2.start))
      return true;
   if (o2 == Orientation::CoLinear and on_segment_when_colinear(s1, s2.end))
      return true;
   if (o3 == Orientation::CoLinear and on_segment_when_colinear(s2, s1.start))
      return true;
   if (o4 == Orientation::CoLinear and on_segment_when_colinear(s2, s1.end))
      return true;

   return false;
}

// implements the Shamos-Hoey algorithm for checking if there exists an
// intersection amongst n line segments
bool gis::exists_intersection(const std::vector<LineSegment>& segments)
{
   struct EndPoint
   {
      enum class Type : bool
      {
         Left,
         Right,
      };

      const Point& point() const
      {
         return type == Type::Left ? segment->start : segment->end;
      }

      Type                                     type;
      std::vector<LineSegment>::const_iterator segment;
   };

   std::vector<EndPoint> points {};
   points.reserve(segments.size() * 2);
   for (auto iter {std::begin(segments)}; iter != std::end(segments); ++iter)
   {
      points.push_back({EndPoint::Type::Left, iter});
      points.push_back({EndPoint::Type::Right, iter});
   }

   std::ranges::sort(
       points,
       [](const EndPoint& lhs, const EndPoint& rhs)
       {
          return lhs.point() == rhs.point()
                   ? lhs.type == EndPoint::Type::Left
                   : lex_comp_less(lhs.point(), rhs.point());
       });

   auto is_above = [](const auto& lhs, const auto& rhs)
   {
      return rhs->start.y < lhs->start.y;
   };
   std::multiset<std::vector<LineSegment>::const_iterator, decltype(is_above)>
        encountered_lines {is_above};
   auto above_or_below_intersects = [&encountered_lines](const auto& segment)
   {
      if (segment != std::begin(encountered_lines))
      {
         const auto above_segment {std::prev(segment)};
         if (intersects(**above_segment, **segment))
            return true;
      }

      if (segment != std::prev(std::end(encountered_lines)))
      {
         const auto below_segment {std::next(segment)};
         if (intersects(**segment, **below_segment))
            return true;
      }
      return false;
   };
   for (const auto& end_point : points)
   {
      switch (end_point.type)
      {
         case EndPoint::Type::Left: {
            const auto inserted {encountered_lines.insert(end_point.segment)};

            if (above_or_below_intersects(inserted))
               return true;
         }
         break;
         case EndPoint::Type::Right: {
            const auto extracted {encountered_lines.find(end_point.segment)};
            if (above_or_below_intersects(extracted))
               return true;
            encountered_lines.erase(extracted);
         }
         break;
         default: throw;
      }
   }
   return false;
}