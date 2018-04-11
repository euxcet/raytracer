#ifndef SHAPE_PLANE_H
#define SHAPE_PLANE_H

#include "shape.h"

namespace Raytracer {

class Plane : public Shape {
public:
	Plane(const Normal3 &normal, const Point3 &origin) :
		Shape(), normal(normal), origin(origin) { dist = Vector3(origin).Length(); }

	bool Intersect(const Ray& ray, Intersection *isc) const;
	Normal3 GetNormal(const Point3& pos) const { return normal; }
	Point3 GetCenter() const { return origin; }
    AABB GetAABB() const { puts("Can't GetAABB from class [Plane] "); return AABB(); }
    pair<float, float> Coordinate(const Point3& pos) const; // TODO: modify pair<int,int> to Point2

private:
    Normal3 normal;
    Point3 origin;
    float dist;
};

Shape* CreatePlaneShape(const Normal3 &normal, const Point3 &origin);


}

#endif
