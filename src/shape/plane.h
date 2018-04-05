#ifndef SHAPE_PLANE_H
#define SHAPE_PLANE_H

#include "shape.h"

namespace Raytracer {

class Plane : public Shape {
public:
	Plane(const Normal3 &normal, float dist) :
		Shape(), normal(normal), dist(dist) {}

	bool Intersect(const Ray& ray, Intersection *isc) const;
	Normal3 GetNormal(const Point3& pos) const { return normal; }
	Point3 GetCenter() const { return Point3(0, 0, 0) + Vector3(dist * normal); }
    pair<float, float> Coordinate(const Point3& pos) const; // TODO: modify pair<int,int> to Point2

private:
    Normal3 normal;
    float dist;
};

Plane* CreatePlaneShape(const Normal3 &normal, float dist);


}

#endif
