#include "shape/sphere.h"

namespace Raytracer {

bool Sphere::Intersect(const Ray& ray, Intersection *isc) const {
	Vector3 v = ray.GetOrigin() - center;
	float b = -Dot(v, ray.GetDirection());
	float det = (b * b) - Dot(v, v) + radius * radius;

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

AABB Sphere::GetAABB() const {
	//TODO;
	return AABB();
}

pair<float, float> Sphere::Coordinate(const Point3& pos) const {
	Vector3 v = Normalize(pos - GetCenter());
	float phi = (atan2(v.y, v.x) + PI) / (2. * PI);
	float theta = (atan2(v.z, sqrt(v.x * v.x + v.y * v.y)) + PI / 2.) / PI;
	return make_pair(phi, theta);
}

Shape* CreateSphereShape(float x, float y, float z, float radius) {
	return new Sphere(Point3(x, y, z), radius);
}


}
