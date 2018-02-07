#include "raytracer.h"

namespace Raytracer {

void Engine::Init() {
}

Primitive* Engine::Raytrace(const Ray& ray, Color& acc, int depth, float index, float& dist) {
	if (depth > TRACEDEPTH) return 0;
	dist = BIGFLOAT;
	vec3 pi;
	Primitive* prim = 0;
	int result;
	for(int i = 0; i < scene -> GetPrimcnt(); i++) {
		Primitive* pr = scene -> GetPrimitive(i);
		int res;
		if ((res = pr -> Intersect(ray, dist))) {
			prim = pr;
			result = res;
		}
	}
	if (!prim) return 0;
	if (prim -> IsLight()) {
		// White Light
		acc = Color(1, 1, 1);
	}
	else {
		pi = ray.GetOrigin() + ray.GetDirection() * dist;
		for(int i = 0; i < scene -> GetPrimcnt(); i++) {
			Primitive* p = scene -> GetPrimitive(i);
			if (p -> IsLight()) {
				Primitive* light = p;
				
				// diffuse shading
				float shade = 1.;
				if (light -> GetType() == Primitive::SPHERE) {
					vec3 L = ((Sphere*)light) -> GetCenter() - pi;
					float tdist = L.Length();
					L.Normalize();
					Ray r = Ray(pi + L * EPS, L);
					for (int j = 0; j < scene -> GetPrimcnt(); j++) {
						Primitive* pr = scene -> GetPrimitive(j);
						if ((pr != light) && (pr -> Intersect(r, tdist))) {
							shade = 0;
							break;
						}
					}
				}

				vec3 L = ((Sphere*)light) -> GetCenter() - pi;
				L.Normalize();
				vec3 N = prim -> GetNormal(pi);
				if (prim -> GetMaterial() -> GetDiffuse() > 0) {
					float dot = L.Dot(N);
					if (dot > 0) {
						float diff = dot * prim -> GetMaterial() -> GetDiffuse() * shade;
						acc += diff * ( prim -> GetMaterial() -> GetColor() ) * ( light -> GetMaterial() -> GetColor() );
					}
				}


				//specular component
				if (prim -> GetMaterial() -> GetSpecular() > 0) {
					vec3 V = ray.GetDirection();
					vec3 R = L - 2.0f * L.Dot(N) * N;
					float dot = V.Dot(R);
					if (dot > 0) {
						float spec = powf(dot, 20) * prim -> GetMaterial() -> GetSpecular() * shade;
						acc += spec * light -> GetMaterial() -> GetColor();
					}
				}
			}
		}


		float refl = prim -> GetMaterial() -> GetReflection();
		if (refl > 0 && depth < TRACEDEPTH) {
			vec3 N = prim -> GetNormal(pi);
			vec3 R = ray.GetDirection() - 2.0f * ray.GetDirection().Dot(N) * N;
			Color rcol(0, 0, 0);
			float dist;
			Raytrace(Ray(pi + R * EPS, R), rcol, depth + 1, index, dist);
			acc += refl * rcol * prim -> GetMaterial() -> GetColor();
		}

		float refr = prim -> GetMaterial() -> GetRefraction();
		if (refr > 0 && depth < TRACEDEPTH) {
			float rindex = prim -> GetMaterial() -> GetRefrIndex();
			float n = index / rindex;
			vec3 N = prim -> GetNormal(pi) * (float) result;
			float cosI = -N.Dot(ray.GetDirection());
			float cosT2 = 1.0f - n * n * (1.0f - cosI * cosI);
			if (cosT2 > 0.0f) {
				vec3 T = (n * ray.GetDirection()) + (n * cosI - sqrtf(cosT2)) * N;
				Color rcol(0, 0, 0);
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

bool Engine::Render() {
	out.open("1.ppm");

	vec3 o(0, 0, -5);
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
					vec3 dir = vec3(px, py, 0) - o;
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
