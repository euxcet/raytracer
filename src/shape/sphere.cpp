#include "shape/sphere.h"

namespace Raytracer {

bool Sphere::Intersect(const Ray& ray, Intersection *isc) const {
	Vector3 v = ray.GetOrigin() - center;
	float b = -Dot(v, ray.GetDirection());
	float det = (b * b) - Dot(v, v) + radius * radius;
//	cout << ray.origin << " " << ray.direction << endl;

	if (det > 0) {
		det = sqrtf(det);
		float i1 = b - det;
		float i2 = b + det;
		if (i2 > 0) {
			if (i1 < 0) {
				if (i2 < ray.tmax) {
					isc -> dist = i2;
					isc -> hit = -1;
					isc -> p = ray(i2);
					isc -> n = GetNormal(isc -> p) * -1;
					return true;
				}
			}
			else if (i1 < ray.tmax) {
				isc -> dist = i1;
				isc -> hit = 1;
				isc -> p = ray(i1);
				isc -> n = GetNormal(isc -> p);
				return true;
			}
		}
	}
	return false;
}

Shape* CreateSphereShape(float x, float y, float z, float radius) {
	return new Sphere(Point3(x, y, z), radius);
}


}
