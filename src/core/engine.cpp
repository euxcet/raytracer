#include "engine.h"

namespace Raytracer {

Color Engine::Raytrace(const Ray& ray, int depth, float index) {
	if (depth > TRACEDEPTH) return Color(0, 0, 0);

    Intersection isc;
    bool result = scene -> Intersect(ray, &isc);
    if (!result) return Color(0, 0, 0);

    if (isc.light != NULL) return isc.light -> GetColor();
    Color res;
    res = scene -> Le(ray, isc);
    res += isc.primitive -> GetMaterial() -> GetReflection()
                * SpawnReflectionRay(ray, isc, depth, index);
    res += isc.primitive -> GetMaterial() -> GetRefraction()
                * SpawnRefractionRay(ray, isc, depth, index);

    return res;
}

Color Engine::SpawnReflectionRay(const Ray &ray, Intersection isc, int depth, float index) {
    Normal3 N = isc.n;
    Vector3 D = ray.GetDirection();
    Vector3 R = Normalize(D - 2.0f * Dot(D, N) * N);
    Color pcolor = isc.primitive -> GetMaterial() -> GetColor();
    return Raytrace(Ray(isc.p + R * EPS, R), depth + 1, index) * pcolor;
}

Color Engine::SpawnRefractionRay(const Ray &ray, Intersection isc, int depth, float index) {
    float rindex = isc.primitive -> GetMaterial() -> GetRefrIndex();
    float n = index / rindex;
    Normal3 N = isc.n;
    float cosI = -Dot(N, ray.GetDirection());
    float cosT2 = 1.0f - n * n * (1.0f - cosI * cosI);
    if (cosT2 > 0.0f) {
        Vector3 T = Normalize((n * ray.GetDirection()) + (n * cosI - sqrtf(cosT2)) * N);
        Color pcolor = isc.primitive -> GetMaterial() -> GetColor();
        Color c = Raytrace(Ray(isc.p + T * EPS, T), depth + 1, index);
		if (isc.hit == 1) return c * pcolor;
		Color absorb = (-0.15 * isc.dist) * pcolor;
		Color trans = Color(exp(absorb.r), exp(absorb.g), exp(absorb.b));
		return c * trans;
    }
	return Color(0, 0, 0);
}

bool Engine::Render() {
	out.open("1.ppm");
	Point3 o(0, 0, -5);
	out << "P3\n" << width << " " << height << "\n255\n";
	int ns = 1;
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			Color sum(0, 0, 0);
			for(int p = -ns + 1; p < ns; p++) {
				for(int q = -ns + 1; q < ns; q++) {
					Color acc(0, 0, 0);
					double px = -4. + x * 0.01 + p * (0.01 / ns);
					double py = 3. - y * 0.01 - q * (0.01 / ns);
					Vector3 dir = Normalize(Point3(px, py, 0) - o);
					Ray r(o, dir);
					float dist;
                    sum += Raytrace(r, 1, 1.0f);
				}
			}
			sum /= (2 * ns - 1) * (2 * ns - 1);
			int red = fmin(sum.r * 255,255.);
			int green = fmin(sum.g * 255,255.);
			int blue = fmin(sum.b * 255,255.);
			out << red << " " << green << " " << blue << endl;
		}
	}

	return true;
}


};
