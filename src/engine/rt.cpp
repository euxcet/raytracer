#include "engine/rt.h"

namespace Raytracer {

Color RaytracerEngine::Raytrace(const Ray& ray, int depth, float index) {
	if (depth >= TRACEDEPTH) return Color(0, 0, 0);

    Intersection isc;
    bool result = scene -> Intersect(ray, &isc);
    if (!result) return Color(0, 0, 0);

    if (isc.light != NULL) return isc.light -> GetColor();

    Color res = Color();
    res = scene -> Le(ray, isc);

	if (isc.primitive -> GetMaterial() -> GetReflection() > EPS)
	    res += isc.primitive -> GetMaterial() -> GetReflection()
	                * SpawnReflectionRay(ray, isc, depth, index);

	if (isc.primitive -> GetMaterial() -> GetRefraction() > EPS)
	    res += isc.primitive -> GetMaterial() -> GetRefraction()
	                * SpawnRefractionRay(ray, isc, depth, index);

    return res;
}

Color RaytracerEngine::SpawnReflectionRay(const Ray &ray, Intersection isc, int depth, float index) {
    Normal3 N = isc.n;
    Vector3 D = ray.GetDirection();
    Vector3 R = Normalize(D - 2.0f * Dot(D, N) * N);

    Color pcolor = isc.primitive -> GetMaterial() ->
					GetColor(isc.primitive -> GetShape() -> Coordinate(isc.p));

    return Raytrace(Ray(isc.p + R * EPS, R), depth + 1, index) * pcolor;
}

Color RaytracerEngine::SpawnRefractionRay(const Ray &ray, Intersection isc, int depth, float index) {
    float rindex = isc.primitive -> GetMaterial() -> GetRefrIndex();
    float n = index / rindex;
	if (isc.hit == 1)
		n = 1 / rindex;
	else
		n = rindex;

    Normal3 N = isc.n;
    float cosI = -Dot(N, ray.GetDirection());
    float cosT2 = 1.0f - n * n * (1.0f - cosI * cosI);
    if (cosT2 > EPS) {
        Vector3 T = Normalize((n * ray.GetDirection()) + (n * cosI - sqrtf(cosT2)) * N);
        Color c = Raytrace(Ray(isc.p + T * EPS, T), depth + 1, rindex);
		if (isc.hit == 1) return c;
		Color absorb = -isc.dist * isc.primitive -> GetMaterial() -> GetAbsorb();
		Color trans = Color(exp(absorb.r), exp(absorb.g), exp(absorb.b));
		return c * trans;
    }
	return Color(0, 0, 0);
}

bool RaytracerEngine::Render() {
	for(int i = 0; i < width; i++) {
		for(int j = 0; j < height; j++) {
			Ray r = camera -> Emit(i, j);
			Color col = Color();
			for(int t = 0; t < SAMPLE; t++) {
				col += Raytrace(camera -> Emit(i, j), 0, 1.0f);
			}
			camera -> SetColor(i, j, col / SAMPLE);
		}
		cout << (1. * i) / width << endl;
	}
	camera -> print();
	return true;
}

Engine* CreateRaytracerEngine(Scene *scene, Camera *camera, int width, int height) {
    return new RaytracerEngine(scene, camera, width, height);
}

};
