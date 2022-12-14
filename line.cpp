#include "line.h"
#include <set>

bool gis::intersects(const LineSegment& l1, const LineSegment& l2)
{
   auto on_segment_when_colinear =
       [](const LineSegment& line, const Point& point)
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