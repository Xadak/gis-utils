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
### Rectangle
A rectangle is defined by two corner points. It stores its top left and bottom right points and can calculate the other two corners.
```
gis::Rectangle rect{{1, 1}, {3, 3}};
assert(rect.topLeft() == {1, 3});
assert(rect.topRight() == {3, 3});
assert(rect.bottomLeft() == {1, 1});
assert(rect.bottomRight() == {3, 1});
```
