#include "engine/ppm.h"
#include <omp.h>

namespace Raytracer {


void PPMEngine::SpawnReflectionRay(const Ray &ray, Intersection isc, int depth,
                                   float index, Vector3 weight, int id) {
    Normal3 N = isc.n;
    if (Dot(ray.direction, N) > 0) N = -N;
    Vector3 D = ray.GetDirection();
    Vector3 R = Normalize(D - 2.0f * Dot(D, N) * N);

    Color pcolor = isc.primitive -> GetMaterial() ->
					GetColor(isc.primitive -> GetShape() -> Coordinate(isc.p));

    Raytrace(Ray(isc.p + R * EPS, R), depth + 1, index, weight * pcolor, id);
}

void PPMEngine::SpawnRefractionRay(const Ray &ray, Intersection isc, int depth,
                                   float index, Vector3 weight, int id) {
    float rindex = isc.primitive -> GetMaterial() -> GetRefrIndex();
    float n = 1 / rindex;
    Normal3 N = isc.n;
    if (Dot(ray.direction, N) > 0) N = -N;
    if (index == rindex) {
        n = rindex;
        rindex = 1;
    }
    float cosI = -Dot(N, ray.GetDirection());
    float cosT2 = 1.0f - n * n * (1.0f - cosI * cosI);
    if (cosT2 > EPS) {
        Vector3 T = Normalize((n * ray.GetDirection()) + (n * cosI - sqrtf(cosT2)) * N);
        if (rindex != 1) {
            Raytrace(Ray(isc.p + T * EPS, T), depth + 1, rindex, weight, id);
        }
        else {
    		Color absorb = -isc.dist * isc.primitive -> GetMaterial() -> GetAbsorb();
    		Color trans = Color(exp(absorb.r), exp(absorb.g), exp(absorb.b));
            Raytrace(Ray(isc.p + T * EPS, T), depth + 1, rindex, weight * trans, id);
        }
    }
}

Color PPMEngine::Raytrace(const Ray& ray, int depth, float index) {
    if (weight.Power() < EPS) return;

	if (depth >= TRACEDEPTH) return;
    Intersection isc;
    bool result = scene -> Intersect(ray, &isc);
    if (!result) return;

    float diff = isc.primitive -> GetMaterial() -> GetDiffuse();
    Color color = isc.primitive -> GetMaterial() ->
					GetColor(isc.primitive -> GetShape() -> Coordinate(isc.p));


    Color res = color * diff * photonmap -> GetColor();

    float refl = isc.primitive -> GetMaterial() -> GetReflection();
    float refr = isc.primitive -> GetMaterial() -> GetRefraction();
    if (refl > 0) SpawnReflectionRay(ray, isc, depth, index, weight * refl, id);
    if (refr > 0) SpawnRefractionRay(ray, isc, depth, index, weight * refr, id);
}

bool PPMEngine::PhotonDiffusion(const Photon &photon, const Intersection &isc,
                                int depth, float index, float &prob) {
    Color color = isc.primitive -> GetMaterial() ->
					GetColor(isc.primitive -> GetShape() -> Coordinate(isc.p));
    float diff = isc.primitive -> GetMaterial() -> GetDiffuse();
    float spec = isc.primitive -> GetMaterial() -> GetSpecular();
    if ((diff) * color.Power() <= RAND() * prob) {
        prob -= (diff) * color.Power();
        return false;
    }
    float x, y, z;
    do {
        x = RAND() * 2 - 1;
        y = RAND() * 2 - 1;
        z = RAND() * 2 - 1;
    } while (x*x + y*y + z*z > 1 || x*x + y*y + z*z < EPS || Dot(Vector3(x, y, z), isc.n) < EPS);
    Color pcolor = photon.color * color / color.Power();
    Photontrace(Photon(photon.position, Normalize(Vector3(x, y, z)), pcolor), depth + 1, index);
    return true;
}
bool PPMEngine::PhotonReflection(const Photon &photon, const Intersection &isc,
                                 int depth, float index, float &prob) {
    Color color = isc.primitive -> GetMaterial() ->
					GetColor(isc.primitive -> GetShape() -> Coordinate(isc.p));
    float refl = isc.primitive -> GetMaterial() -> GetReflection();
    if (refl * color.Power() <= RAND() * prob) {
        prob -= refl * color.Power();
        return false;
    }
    Normal3 N = isc.n;
    if (Dot(photon.direction, N) > 0) N = -N;
    Vector3 D = photon.direction;
    Vector3 R = Normalize(D - 2.0f * Dot(D, N) * N);
    Vector3 pcolor = photon.color * color / color.Power();
    Photontrace(Photon(photon.position, R, pcolor), depth + 1, index);
    return true;
}
bool PPMEngine::PhotonRefraction(const Photon &photon, const Intersection &isc,
                                 int depth, float index, float &prob) {
    Color color = isc.primitive -> GetMaterial() ->
					GetColor(isc.primitive -> GetShape() -> Coordinate(isc.p));
    float refr = isc.primitive -> GetMaterial() -> GetRefraction();
    float rindex = isc.primitive -> GetMaterial() -> GetRefrIndex();
    Color trans = Color(1., 1., 1.);
    if (index == rindex) {
		Color absorb = -isc.dist * isc.primitive -> GetMaterial() -> GetAbsorb();
		trans = Color(exp(absorb.r), exp(absorb.g), exp(absorb.b));
    }
    if (refr * trans.Power() <= RAND() * prob) {
        prob -= refr * trans.Power();
        return false;
    }
    float n = 1 / rindex;
    Normal3 N = isc.n;
    if (Dot(photon.direction, N) > 0) N = -N;
    if (index == rindex) {
        n = rindex;
        rindex = 1;
    }
    float cosI = -Dot(N, photon.direction);
    float cosT2 = 1.0f - n * n * (1.0f - cosI * cosI);
    if (cosT2 > EPS) {
        Vector3 T = Normalize((n * photon.direction) + (n * cosI - sqrtf(cosT2)) * N);
        Color pcolor = photon.color * trans / trans.Power();
        Photontrace(Photon(photon.position, T, pcolor), depth + 1, rindex);
    }
    return true;
}
void PPMEngine::Photontrace(const Photon &photon, int depth, float index) {
    if (photon.color.Power() < EPS) return;
    if (depth >= PHOTON_DEPTH) return;
    Intersection isc;
    bool result = scene -> Intersect(Ray(photon.position, photon.direction), &isc);
    if (!result) return;

    Photon hitphoton = Photon(isc.p, photon.direction, photon.color);
    if (isc.primitive -> GetMaterial() -> GetDiffuse() > EPS) {
        photons.push_back(hitphoton);
    }

    float prob = 1.;
    if (PhotonDiffusion(hitphoton, isc, depth, index, prob)) return;
    if (PhotonReflection(hitphoton, isc, depth, index, prob)) return;
    if (PhotonRefraction(hitphoton, isc, depth, index, prob)) return;
    return;
}

bool PPMEngine::Render() {
    vector<Photon*> photons = scene -> EmitPhotons(PHOTON_COUNT);
    for(int i = 0; i < photons.size(); i++) {
        Photontrace(*photons[i], 0, 1);
    }
    photonmap = new PhotonMap(photons);

    for(int i = 0; i < width; i++)
        for(int j = 0; j < height; j++) {
            Color color = Raytrace(camera -> Emit(i, j), 0, 1);
            camera -> SetColor(i, j, color);
        }

    delete photonmap;

	return true;
}

Engine* CreatePMEngine(Scene *scene, Camera *camera, int width, int height, int p) {
    return new PEngine(scene, camera, width, height, p);
}

};
