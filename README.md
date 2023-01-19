
# GIS-UTILS

A library providing basic primitives for use in gis applications.

 **Warning: Do not use in production code!** This library was developed as support code for a university course assignment. As such, a lot of the features you might expect from a production grade library of this type may be missing and will probably not be added. Also, while the code runs natively and I attempted to optimize it as best I could it has not been performance tested for large numbers of elements. Use at your own risk.

## Coordinate System

The library uses the Point struct comprised of an x and y coordinate to represent a point in 2D space.
The coordinates are stored as a double (aliased coord_t) with positive x pointing right and positive y pointing upwards.

## C++ API

### Point

A point in 2-D space is defined by an x and y coordinate like so:

``` C++
gis::Point p{1, 3.5};
```

Typical 2D linear algebra operators are overloaded for point (vector addition and multiplication, scalar multiplication and division, dot product).

``` C++
gis::Point p1 {0, 1};
gis::Point p2 {3, 4};

assert(p1 + p2 == {3, 5});
assert(p1 - p2 == {-3, -3});
assert(2 * p1  == {0, 2});
assert(1.5 * p2  == {4.5, 6});
assert(p1 / 2  == {0, 1.5});
assert(dot_product(p1, p2) == 4);
```

Functions for calculating the Euclidean distance and distance squared of two points are provided (distance squared is provided for optimization purposes).

``` C++
assert(distance({0, 0}, {2, 0}) == 2);
assert(distance({0, 0}, {4, 3}) == 5);
assert(distance_squared({0, 0}, {4, 3}) == 25);
```

A lexicographical comparison function is provided called lex_comp_less. The Point with the smallest x coordinate is considered less and the y coordinates are compared in the case of a tie:

``` C++
assert(gis::lex_comp_less({1, 2}, {2, 1}));
assert(gis::lex_comp_less({1, 2}, {1, 2.5}));
```

A query for the orientation of three given Points is also provided, returning an enum value:

``` C++
assert(gis::orientation({1, 0}, {2, 0}, {3, 0}) == gis::Orientation::Colinear);
assert(gis::orientation({0, 0}, {1, 0}, {2, -1}) == gis::Orientation::CW);
assert(gis::orientation({0, 0}, {1, 0}, {2, 1}) == gis::Orientation::CCW);
```

### Geometry

A variant of all the different primitives provided by the library. Provides the following common interface:

``` C++
// returns the minimum bounding rectangle of the geometry
Rectangle MBR(const Geometry&);

// returns a vector of the line segments that comprise the geometry
std::vector<LineSegment> to_segments(const Geometry&);

// returns true if the two geometries intersect and false if the two geometries are disjoint OR if one of them is fully contained by the other. The southern and western boundaries of each geometry are included and the northern and eastern are excluded.
bool intersects(const Geometry&, const Geometry&);

// returns true if lhs contains rhs. The optional bool parameter should be true if it is known that the two geometries do not intersect to avoid an expensive check inside the function.
bool contains( const Geometry& lhs, const Geometry& rhs, bool            certainly_dont_intersect = false);

// returns the smallest distance between p and one of the points of g. If p is contained in g the return value is zero.
coord_t min_distance(const Geometry& g, const Point& p);

// returns the smallest distance between a point of lhs and a point of rhs. If one of the geometries intersects or is contained by the other, the return value is zero.
inline coord_t min_distance(const Geometry& lhs, const Geometry& rhs);

enum class SpatialRelationship
{
   Disjoint,
   Intersecting,
   AContainsB,
   BContainsA,
};

// returns the spatial relationship between two geometries as one of the values of the above enum.
SpatialRelationship spacial_relationship(const Geometry& a, const Geometry& b)

```

### Rectangle

A rectangle is defined by two corner points. It stores its top left and bottom right points and can calculate the other two corners.

``` C++
gis::Rectangle rect{{1, 1}, {3, 3}};
assert(rect.topLeft() == {1, 3});
assert(rect.topRight() == {3, 3});
assert(rect.bottomLeft() == {1, 1});
assert(rect.bottomRight() == {3, 1});
```

You can check if a point is contained in a rectangle:

``` C++
assert(contains(gis::Rectangle{{1, 1}, {3, 3}}, gis::Point{2, 2}));
```

### LineSegment

A line segment is defined by two points.

``` C++
LineSegment segment{{2, 1}, {1, 1}};
assert(segment.start == {2, 1});
assert(segment.end == {1, 1});
```

### PolyLine

A poly-line is defined by a collection of points. Depending on the LineType passed as a template parameter, a poly-line can be open, closed or filled (Polygon is an alias of PolyLine\<Filled>):

```C++
PolyLine<LineType::Open> p1{{1, 1}, {1, 2}, {3, 0.5}};
PolyLine<LineType::Closed> p2{{0, 0}, {-1, 3}, {-2, 3.5}};
Polygon p3{{-1, 0}, {0, 1}, {1, 0}};
```

**WARNING**: Self intersecting poly-lines are not supported as the intersection query detects the self intersections.
