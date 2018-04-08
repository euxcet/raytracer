#ifndef SHAPE_TRIANGLE_H
#define SHAPE_TRIANGLE_H

#include "shape.h"

namespace Raytracer {

class Triangle : public Shape {
public:
	Triangle(const Point3 &v0, const Point3 &v1, const Point3 &v2) :
        Shape(), v0(v0), v1(v1), v2(v2) {
            Vector3 v0v1 = Normalize(v1 - v0);
            Vector3 v0v2 = Normalize(v2 - v0);
            n = Normal3(Normalize(Cross(v0v1, v0v2)));
        }

	bool Intersect(const Ray& ray, Intersection *isc) const;
	Normal3 GetNormal(const Point3& pos) const { return n; }
	Point3 GetCenter() const { return (v0 + v1 + v2) / 3.; }
    AABB GetAABB() const;
    pair<float, float> Coordinate(const Point3& pos) const; // TODO: modify pair<int,int> to Point2

private:
    Point3 v0;
    Point3 v1;
    Point3 v2;
    Normal3 n;
};

Shape* CreateTriangleShape(const Point3 &v0, const Point3& v1, const Point3 &v2);


}

#endif
