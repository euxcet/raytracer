#include "engine.h"

namespace Raytracer {


int Engine::FindNearest(const Ray& ray, float& dist, Primitive*& prim) {
	int result = MISS;
	for(int i = 0; i < scene -> GetPrimcnt(); i++) {
		shared_ptr<Primitive> pr = scene -> GetPrimitive(i);
		int res;
		if ((res = pr -> Intersect(ray, dist))) {
			prim = pr;
			result = res;
		}
	}
	return result;
}

Primitive* Engine::Raytrace(const Ray& ray, Color& acc, int depth, float index, float& dist) {
	if (depth > TRACEDEPTH) return 0;
	dist = BIGFLOAT;
	Vector3 pi;

	Primitive* prim = 0;
	int result = FindNearest(ray, dist, prim);
	if (result == MISS) return 0;

	if (prim -> IsLight()) {
		// Light's color
		acc = prim -> GetMaterial() -> GetColor();
	}
	else {
		pi = ray.GetOrigin() + ray.GetDirection() * dist;
		for(int i = 0; i < scene -> GetPrimcnt(); i++) {
			shared_ptr<Primitive> p = scene -> GetPrimitive(i);
			if (p -> IsLight()) {
				Primitive* light = p;

				// diffuse shading

				Vector3 L;
				float shade = CalcShade(light, pi, L);

				if (shade > 0) {
					// diffuse shading
					Vector3 L = (light)->GetCenter() - pi;
					L.Normalize();

					Vector3 N = prim -> GetNormal(pi);
					if (prim -> GetMaterial() -> GetDiffuse() > 0) {
						float dot = Dot(L, N);
						if (dot > 0) {
							float diff = dot * prim -> GetMaterial() -> GetDiffuse() * shade;
							acc += diff * ( prim -> GetMaterial() -> GetColor() ) * ( light -> GetMaterial() -> GetColor() );
						}
					}


					//specular component
					if (prim -> GetMaterial() -> GetSpecular() > 0) {
						Vector3 V = ray.GetDirection();
						Vector3 R = L - 2.0f * Dot(L, N) * N;
						float dot = Dot(V, R);
						if (dot > 0) {
							float spec = powf(dot, 20) * prim -> GetMaterial() -> GetSpecular() * shade;
							acc += spec * light -> GetMaterial() -> GetColor();
						}
					}
				}


			}
		}


		// TODO: more rays
		float refl = prim -> GetMaterial() -> GetReflection();
		if (refl > 0 && depth < TRACEDEPTH) {
			Vector3 N = prim -> GetNormal(pi);
			Vector3 R = ray.GetDirection() - 2.0f * Dot(ray.GetDirection(), N) * N;
			R.Normalize();
			Color rcol(0, 0, 0);
			float dist;
			Raytrace(Ray(pi + R * EPS, R), rcol, depth + 1, index, dist);
			acc += refl * rcol * prim -> GetMaterial() -> GetColor();
		}

		float refr = prim -> GetMaterial() -> GetRefraction();
		if (refr > 0 && depth < TRACEDEPTH) {
			float rindex = prim -> GetMaterial() -> GetRefrIndex();
			float n = index / rindex;
			Vector3 N = prim -> GetNormal(pi) * (float) result;
			float cosI = -Dot(N, ray.GetDirection());
			float cosT2 = 1.0f - n * n * (1.0f - cosI * cosI);
			if (cosT2 > 0.0f) {
				Vector3 T = (n * ray.GetDirection()) + (n * cosI - sqrtf(cosT2)) * N;
				Color rcol(0, 0, 0);
				T.Normalize();
				float dist;
				Raytrace(Ray(pi + T * EPS, T), rcol, depth + 1, rindex, dist);
				Color absorbance = prim -> GetMaterial() -> GetColor() * 0.15f * (-dist);
				Color transparency = Color( expf(absorbance.r), expf(absorbance.g),
						expf(absorbance.b));

				acc += rcol * transparency;
			}
		}
	}
	return prim;
}

float Engine::CalcShade(Primitive* light, Vector3 pi, Vector3& dir) {
	float shade = 1.;
	if (light -> GetType() == Primitive::SPHERE) {
		dir = (light) -> GetCenter() - pi;
		float tdist = dir.Length();
		dir.Normalize();
		Primitive* pr = 0;
		FindNearest(Ray(pi + dir * EPS, dir), tdist, pr);
		if (pr != light) shade = 0;
	}
	else if (light -> GetType() == Primitive::BOX) {
		shade = 0.;
		Box* box = (Box*)light;
		dir = box -> GetPos() + 0.5 * box -> GetSize() - pi;
		dir.Normalize();

		//SAMPLE
		int SAMPLE = 2;
		for (int i = 0; i < SAMPLE; i++) for(int j = 0; j < SAMPLE; j++) {
			Vector3 des = box -> GetPos() + Vector3(box -> GetSize().x / SAMPLE * i, 0, box -> GetSize().z / SAMPLE * j);
			Vector3 d = des - pi;
			float tdist = d.Length();
			d.Normalize();
			Primitive* pr = 0;
			if (FindNearest(Ray(pi + d * EPS, d), tdist, pr))
				if (pr == light) shade += 1. / (SAMPLE * SAMPLE);
		}

	}
	return shade;

}

bool Engine::Render() {
	out.open("1.ppm");
	Vector3 o(0, 0, -5);
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
					Vector3 dir = Vector3(px, py, 0) - o;
					dir.Normalize();
					Ray r(o, dir);

					float dist;
					Primitive* prim = Raytrace(r, acc, 1, 1.0f, dist);
					sum += acc;

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
