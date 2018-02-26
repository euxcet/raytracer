#include "primitive.h"

namespace Raytracer {

int Sphere::Intersect(const Ray& ray, float& dist) {
	vec3 v = ray.GetOrigin() - center;
	float b = -v.Dot(ray.GetDirection());
	float det = (b * b) - v.Dot(v) + radius * radius;
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

int PlanePrim::Intersect(const Ray& ray, float& dist) {
	float d = plane.N.Dot(ray.GetDirection());
	if (d != 0) { // TODO: eps
		float dis = -(plane.N.Dot(ray.GetOrigin()) + plane.D) / d;
		if (dis > 0) {
			if (dis < dist) {
				dist = dis;
				return HIT;
			}
		}
	}
	return MISS;
}

}
