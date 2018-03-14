#include "shape/sphere.h"

namespace Raytracer {

int Sphere::Intersect(const Ray& ray, float& dist) const {
	Vector3 v = ray.GetOrigin() - center;
	float b = -Dot(v, ray.GetDirection());
	float det = (b * b) - Dot(v, v) + radius * radius;
	int res = MISS;
	if (det > 0) {
		det = sqrtf(det);
		float i1 = b - det;
		float i2 = b + det;
		if (i2 > 0) {
			if (i1 < 0) {
				if (i2 < dist) {
					dist = i2;
					res = INPRIM;
				}
			}
			else if (i1 < dist) {
				dist = i1;
				res = HIT;
			}
		}
	}
	return res;
}


}
