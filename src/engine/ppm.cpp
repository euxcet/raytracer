#include "engine/ppm.h"
#include <omp.h>

namespace Raytracer {

KDTree::KDTree(const vector<HitPoint*> &vhps) {
    hpcount = vhps.size();
    hps = new HitPoint*[hpcount];
    for(int i = 0; i < hpcount; i++)
        hps[i] = vhps[i];
    root = BuildTree(0, hpcount - 1);
}

KDTNode* KDTree::BuildTree(int u, int v) {
    if (u > v) return NULL;
    KDTNode *node = new KDTNode();
    if (u == v) {
        node -> hp = hps[u];
        return node;
    }
    int dim = rand() % 3;
    int mid = (u + v) >> 1;
    if (dim == 0) sort(hps + u, hps + v + 1, cmpX);
    if (dim == 1) sort(hps + u, hps + v + 1, cmpY);
    if (dim == 2) sort(hps + u, hps + v + 1, cmpZ);
    node -> dim = dim;
    node -> hp = hps[mid];
    node -> l = BuildTree(u, mid - 1);
    node -> r = BuildTree(mid + 1, v);
    return node;
}

void KDTree::Update(KDTNode *node, const Photon &photon) {
    if (node == NULL) return;
    HitPoint* hp = node -> hp;
    float curR2 = hp -> R2;
    if ((hp -> isc.p - photon.position).SqrLength() <= curR2 &&
        Dot(hp -> isc.n, photon.direction) < 0) {
        float g = (hp -> tot * ALPHA + ALPHA) / (hp -> tot * ALPHA + 1.0);
        hp -> R2 *= g;
        hp -> tot ++;
        hp -> color = (hp -> color + hp -> weight * photon.color / PI) * g;
    }
    if (node -> dim == 0) {
        if (photon.position.x >= hp -> isc.p.x - sqrt(curR2))
            Update(node -> r, photon);
        if (photon.position.x <= hp -> isc.p.x + sqrt(curR2))
            Update(node -> l, photon);
    }
    else if (node -> dim == 1) {
        if (photon.position.y >= hp -> isc.p.y - sqrt(curR2))
            Update(node -> r, photon);
        if (photon.position.y <= hp -> isc.p.y + sqrt(curR2))
            Update(node -> l, photon);
    }
    else {
        if (photon.position.z >= hp -> isc.p.z - sqrt(curR2))
            Update(node -> r, photon);
        if (photon.position.z <= hp -> isc.p.z + sqrt(curR2))
            Update(node -> l, photon);
    }

}

void PPMEngine::Raytrace(const Ray& ray, int depth, float index, Vector3 weight, int id) {
    if (weight.Power() < EPS) return;

	if (depth >= TRACEDEPTH) return;
    Intersection isc;
    bool result = scene -> Intersect(ray, &isc);
    if (!result) return;

    float diff = isc.primitive -> GetMaterial() -> GetDiffuse();
    Color color = isc.primitive -> GetMaterial() ->
					GetColor(isc.primitive -> GetShape() -> Coordinate(isc.p));
    if (diff > EPS) {
        HitPoint *hp = new HitPoint();
        hp -> weight = weight * diff * color;
        hp -> id = id;
        hp -> ray = ray;
        hp -> isc = isc;
        hp -> tot = 0;
        hp -> R2 = HITRADIUS;
        hps.push_back(hp);
    }
    float refl = isc.primitive -> GetMaterial() -> GetReflection();
    float refr = isc.primitive -> GetMaterial() -> GetRefraction();
    if (refl > 0) SpawnReflectionRay(ray, isc, depth, index, weight * refl, id);
    if (refr > 0) SpawnRefractionRay(ray, isc, depth, index, weight * refr, id);
}

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
            trans = Color(1., 1., 1.);
            Raytrace(Ray(isc.p + T * EPS, T), depth + 1, rindex, weight * trans, id);
        }
    }
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
        trans = Color(1., 1., 1.);
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
        tree -> Update(tree -> root, hitphoton);
    }
    float prob = 1.;
    if (PhotonDiffusion(hitphoton, isc, depth, index, prob)) return;
    if (PhotonReflection(hitphoton, isc, depth, index, prob)) return;
    if (PhotonRefraction(hitphoton, isc, depth, index, prob)) return;
    return;
}

bool PPMEngine::Render() {
    int tot = 0;
    int count = 0;
    while (true) {
        count ++;
        for(int i = 0; i < width * height; i++) {
            float x = (RAND() - 0.5);
            float y = (RAND() - 0.5);
            Raytrace(camera -> Emit(i / height + x, i % height + y), 0, 1, Vector3(1, 1, 1), i);
            cout << i << endl;
        }
        cout << "Hitpoints: " << hps.size() << endl;
        tree = new KDTree(hps);
        puts("Tree done");

        vector<Photon*> photons = scene -> EmitPhotons(PHOTON_COUNT);
        cout << "Photons " << photons.size() << endl;
        for(int i = 0; i < photons.size(); i++) {
            Photontrace(*photons[i], 0, 1);
            tot ++;
            cout << tot << endl;
        }
        cout << tot << endl;


        for(int i = 0; i < hps.size(); i++) {
            HitPoint* hp = hps[i];
            Color col = hp -> color / (PI * hp -> R2 * PHOTON_COUNT);
            camera -> AddColor(hp -> id / height, hp -> id % height, col);
        }
        camera -> print(count);
        for(int i = 0; i < photons.size(); i++)
            delete photons[i];
        photons.clear();
        for(int i = 0; i < hps.size(); i++)
            delete hps[i];
        hps.clear();
        delete tree;

    }
	return true;
}

Engine* CreatePPMEngine(Scene *scene, Camera *camera, int width, int height, int p) {
    return new PPMEngine(scene, camera, width, height, p);
}

};
