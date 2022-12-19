# GIS Primitives:
A library providing basic primitives for use in gis applications.

 **Warning: Do not use in production code!** This library was developed as support code for a university course assignment. As such, a lot of the features you might expect from a production grade library of this type may be missing and will probably not be added. Also, while the code runs natively and I attempted to optimize it as best I could it has not been performance tested for large numbers of elements. Use at your own risk.

## Coordinate System
The library uses the Point struct comprised of an x and y coordinate to represent a point in 2D space.
The coordinates are stored as a double (aliased coord_t) with positive x pointing right and positive y pointing upwards.

## C++ API
### Point
A point in 2-D space is defined by an x and y coordinate like so:
```
gis::Point p{1, 3.5};
```

A lexicographical comparison function is provided called lex_comp_less. The Point with the smallest x coordinate is considered less and the y coordinates are compared in the case of a tie:
```
assert(gis::lex_comp_less({1, 2}, {2, 1}));
assert(gis::lex_comp_less({1, 2}, {1, 2.5}));
```
A query for the orientation of three given Points is also provided, returning an enum value:
```
assert(gis::orientation({1, 0}, {2, 0}, {3, 0}) == gis::Orientation::Colinear);
assert(gis::orientation({0, 0}, {1, 0}, {2, -1}) == gis::Orientation::CW);
assert(gis::orientation({0, 0}, {1, 0}, {2, 1}) == gis::Orientation::CCW);
```
### Geometry
A variant of all the different primitives provided by the library. Provides the following common interface:
```
// returns the minimum bounding rectangle of the geometry
Rectangle MBR(const Geometry&);

// returns a vector of the line segments that comprise the //geometry
std::vector<LineSegment> to_segments(const Geometry&);

// returns true if the two geometries intersect and false if the two geometries are disjoint OR if one of them is fully contained by the other
bool intersects(const Geometry&, const Geometry&);
```
### Rectangle
A rectangle is defined by two corner points. It stores its top left and bottom right points and can calculate the other two corners.
```
gis::Rectangle rect{{1, 1}, {3, 3}};
assert(rect.topLeft() == {1, 3});
assert(rect.topRight() == {3, 3});
assert(rect.bottomLeft() == {1, 1});
assert(rect.bottomRight() == {3, 1});
```
You can check if a point is contained in a rectangle:
```
assert(contains(gis::Rectangle{{1, 1}, {3, 3}}, gis::Point{2, 2}));
```
### LineSegment
A line segment is defined by two points. Of the given points, the one that is lexicographically less than the other is considered the start and the other the end:
```
LineSegment segment{{2, 1}, {1, 1}};
assert(segment.start == {1, 1});
assert(segment.end == {2, 1});
```
### PolyLine
A poly-line is defined by a collection of points. Depending on the LineType passed as a template parameter, a poly-line can be open, closed or filled (Polygon is an alias of PolyLine\<Filled>):
```
PolyLine<LineType::Open> p1{{1, 1}, {1, 2}, {3, 0.5}};
PolyLine<LineType::Closed> p2{{0, 0}, {-1, 3}, {-2, 3.5}};
Polygon p3{{-1, 0}, {0, 1}, {1, 0}};
```
**WARNING**: Self intersecting poly-lines are not supported as the intersection query detects the self intersections.