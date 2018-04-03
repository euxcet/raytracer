#ifndef SHAPE_SPHERE_H
#define SHAPE_SPHERE_H

#include "shape.h"

namespace Raytracer {

class Sphere : public Shape {
public:
	Sphere(const Point3& center, float radius) :
		Shape(), center(center), radius(radius) {}

	bool Intersect(const Ray& ray, Intersection *isc) const;
	Normal3 GetNormal(const Point3& pos) const { return Normal3(Normalize(pos - center)); }

	Point3 GetCenter() const { return center; }
	float GetSqrRadius() const { return radius * radius; }

private:
	Point3 center;
	float radius;
};

Shape* CreateSphereShape(float x, float y, float z, float radius);


}

#endif
