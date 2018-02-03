#include "common.h"
#include "scene.h"
#include "raytracer.h"

namespace Raytracer {

int Sphere::Intersect(Ray& ray, float& dist) {
	vec3 v = ray.GetOrigin() - center;
	flaot b = -v.Dot(ray.GetDirection());
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

int PlanePrim::Intersect(Ray& ray, float& dist) {
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

void Scene::Init() {
	primitive = new Primitive*[100];
	//TODO: make it simpler
	primitive[0] = new PlanePrim(vec3(0, 1, 0), 4.4f);
	primitive[0] -> SetName("plane");
	primitive[0] -> GetMaterial() -> SetReflection(0);
	primitive[0] -> GetMaterial() -> SetDiffuse(1.0f);
	primitive[0] -> GetMaterial() -> SetColor(Color(0.4f, 0.3f, 0.3f));

	primitive[1] = new Sphere(vec3(1, -0.8f, 3), 2.5f);
	primitive[1] -> SetName("sphere one");
	primitive[1] -> GetMaterial() -> SetReflection(0.6f);
	primitive[1] -> GetMaterial() -> SetDiffuse(1.0f);

	primitive[2] = new Sphere(vec3(-5.5f, -0.5f, 7), 2f);
	primitive[2] -> SetName("sphere two");
	primitive[2] -> GetMaterial() -> SetReflection(0.6f);
	primitive[2] -> GetMaterial() -> SetDiffuse(1.0f);

}

};

