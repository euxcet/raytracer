#ifndef SHAPE_SPHERE_H
#define SHAPE_SPHERE_H

#include "primitive.h"

namespace Raytracer {

class Sphere : public Shape {
public:
	Sphere(const Point3& _center, float _radius) :
		center(_center), radius(_radius) {}

	int Intersect(const Ray& ray, float& dist) const;
	Normal3 GetNormal(const Point3& pos) const { return (pos - center) / radius; }


	Point3 GetCenter() const { return center; }
	float GetSqrRadius() const { return radius * radius; }

private:
	Point3 center;
	float radius;
};

std::shared_ptr<Primitive> CreateSpherePrimitive(float x, float y, float z, float radius) {
    return std::make_shared<Sphere>(Vector3(x, y, z), radius);
}

}

#endif
