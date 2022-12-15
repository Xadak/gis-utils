# GIS Primitives:
A library providing basic primitives for use in gis applications.

 **Warning: Do not use in production code!** This library was developed as support code for a university course assignment. As such, a lot of the features you might expect from a production grade library of this type may be missing and will probably not be added. Also, while the code runs natively and I attempted to optimize it as best I could it has not been performance tested for large numbers of elements. Use at your own risk.

## Coordinate System
The library uses the Point struct comprised of an x and y coordinate to represent a point in 2D space.
The coordinates are stored as a double (aliased coord_t) with positive x pointing right and positive y pointing upwards.