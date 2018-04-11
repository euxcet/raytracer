#include "engine/ppm.h"

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

void KDTree::FindNearest(KDTNode *node, Point3 p, float &mdist, float pdist, HitPoint *&hp) {
    if (node == NULL) return;
    float dist = (p - node -> hp -> isc.p).Length();
    if (dist < mdist && dist > pdist)  {
        mdist = dist;
        hp = node -> hp;
    }
    float d;
    if (node -> dim == 0) d = p.x - node -> hp -> isc.p.x;
    if (node -> dim == 1) d = p.y - node -> hp -> isc.p.y;
    if (node -> dim == 2) d = p.z - node -> hp -> isc.p.z;
    if (d < 0) {
        FindNearest(node -> l, p, mdist, pdist, hp);
        if (-d < mdist || hp == NULL) FindNearest(node -> r, p, mdist, pdist, hp);
    }
    else {
        FindNearest(node -> r, p, mdist, pdist, hp);
        if (d < mdist || hp == NULL) FindNearest(node -> l, p, mdist, pdist, hp);
    }
}

void KDTree::Update(KDTNode *node, const Photon &photon) {
    if (node == NULL) return;

    //TODO DOT()

    HitPoint* hp = node -> hp;
//    cout << hp -> isc.p << " " << node -> dim << " " << photon.position << endl;
 //   cout << (hp -> isc.p - photon.position).SqrLength() << endl;
    float curR2 = hp -> R2;
    if ((hp -> isc.p - photon.position).SqrLength() <= curR2) {
        double g = (hp -> tot * ALPHA + ALPHA) / (hp -> tot * ALPHA + 1.0);
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
	if (depth >= TRACEDEPTH) return;
    Intersection isc;
    bool result = scene -> Intersect(ray, &isc);
    if (!result) return;

    double diff = isc.primitive -> GetMaterial() -> GetDiffuse();
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
    SpawnReflectionRay(ray, isc, depth, index, weight * refl, id);
    SpawnRefractionRay(ray, isc, depth, index, weight * refr, id);
}

void PPMEngine::SpawnReflectionRay(const Ray &ray, Intersection isc, int depth,
                                   float index, Vector3 weight, int id) {
    Normal3 N = isc.n;
    Vector3 D = ray.GetDirection();
    Vector3 R = Normalize(D - 2.0f * Dot(D, N) * N);

    Color pcolor = isc.primitive -> GetMaterial() ->
					GetColor(isc.primitive -> GetShape() -> Coordinate(isc.p));

    Raytrace(Ray(isc.p + R * EPS, R), depth + 1, index, weight * pcolor, id);
}

void PPMEngine::SpawnRefractionRay(const Ray &ray, Intersection isc, int depth,
                                   float index, Vector3 weight, int id) {
    float rindex = isc.primitive -> GetMaterial() -> GetRefrIndex();
    float n = (isc.hit == 1) ? (1 / rindex) : rindex;
    Normal3 N = isc.n;
    float cosI = -Dot(N, ray.GetDirection());
    float cosT2 = 1.0f - n * n * (1.0f - cosI * cosI);
    if (cosT2 > EPS) {
        Vector3 T = Normalize((n * ray.GetDirection()) + (n * cosI - sqrtf(cosT2)) * N);
        if (isc.hit == 1) {
            Raytrace(Ray(isc.p + T * EPS, T), depth + 1, rindex, weight, id);
        }
        else {
    		Color absorb = -isc.dist * isc.primitive -> GetMaterial() -> GetAbsorb();
    		Color trans = Color(exp(absorb.r), exp(absorb.g), exp(absorb.b));
            Raytrace(Ray(isc.p + T * EPS, T), depth + 1, rindex, weight * trans, id);
        }
    }
}

bool PPMEngine::PhotonDiffusion(const Photon &photon, const Intersection &isc,
                                int depth, float &prob) {
    // TODO no specular?
    Color color = isc.primitive -> GetMaterial() ->
					GetColor(isc.primitive -> GetShape() -> Coordinate(isc.p));
    double diff = isc.primitive -> GetMaterial() -> GetDiffuse();
    double spec = isc.primitive -> GetMaterial() -> GetSpecular();

    if ((diff + spec) * color.Power() <= rand() % 10000 / 10000. * prob) {
        prob -= (diff + spec) * color.Power();
        return false;
    }

    Vector3 direction = Vector3();
    double x, y, z;
    do {
        x = rand() % 20000 / 10000. - 1;
        y = rand() % 20000 / 10000. - 1;
        z = rand() % 20000 / 10000. - 1;
    } while (x*x + y*y + z*z > 1 || x*x + y*y + z*z < EPS || Dot(Vector3(x, y, z), isc.n) < EPS);

    Color pcolor = photon.color * color / color.Power();

    Photontrace(Photon(photon.position, Vector3(x, y, z), pcolor), depth + 1);
    return true;
}

bool PPMEngine::PhotonReflection(const Photon &photon, const Intersection &isc,
                                 int depth, float &prob) {
    Color color = isc.primitive -> GetMaterial() ->
					GetColor(isc.primitive -> GetShape() -> Coordinate(isc.p));
    double refl = isc.primitive -> GetMaterial() -> GetReflection();

    if (refl * color.Power() <= rand() % 10000 / 10000. * prob) {
        prob -= refl * color.Power();
        return false;
    }

    Normal3 N = isc.n;
    Vector3 D = photon.direction;
    Vector3 R = Normalize(D - 2.0f * Dot(D, N) * N);
    Vector3 pcolor = photon.color * color / color.Power();
    Photontrace(Photon(photon.position, R, pcolor), depth + 1);
    return true;
}

bool PPMEngine::PhotonRefraction(const Photon &photon, const Intersection &isc,
                                 int depth, float &prob) {
    Color color = isc.primitive -> GetMaterial() ->
					GetColor(isc.primitive -> GetShape() -> Coordinate(isc.p));
    double refr = isc.primitive -> GetMaterial() -> GetRefraction();
    Color trans = Color(1., 1., 1.);
    if (isc.hit != 1) {
		Color absorb = -isc.dist * isc.primitive -> GetMaterial() -> GetAbsorb();
		trans = Color(exp(absorb.r), exp(absorb.g), exp(absorb.b));
    }
    if (refr * trans.Power() <= rand() % 10000 / 10000. * prob) {
        prob -= refr * trans.Power();
        return false;
    }

    float rindex = isc.primitive -> GetMaterial() -> GetRefrIndex();
    float n = (isc.hit == 1) ? (1 / rindex) : rindex;
    Normal3 N = isc.n;
    float cosI = -Dot(N, photon.direction);
    float cosT2 = 1.0f - n * n * (1.0f - cosI * cosI);
    if (cosT2 > EPS) {
        Vector3 T = Normalize((n * photon.direction) + (n * cosI - sqrtf(cosT2)) * N);
        Color pcolor = photon.color * trans / trans.Power();
        Photontrace(Photon(photon.position, T, pcolor), depth + 1);
    }
    return true;

}

static int TOT = 0;

void PPMEngine::Photontrace(const Photon &photon, int depth) {
    if (depth >= PHOTON_DEPTH) return;
    Intersection isc;
    bool result = scene -> Intersect(Ray(photon.position, photon.direction), &isc);
    if (!result) return;
    Photon hitphoton = Photon(isc.p, photon.direction, photon.color);
    if (isc.primitive -> GetMaterial() -> GetDiffuse() > EPS) {
    //    cout << hitphoton.color << endl;
    /*
        for(auto hp: hps)
            if ((hp -> isc.p - photon.position).Length() <= hp -> R2) {
                double g = (hp -> tot * ALPHA + ALPHA) / (hp -> tot * ALPHA + 1.0);
                hp -> R2 *= g;
                hp -> tot ++;
                hp -> color = (hp -> color + hp -> weight * photon.color / PI) * g;
            }
            */
        tree -> Update(tree -> root, hitphoton);
    }
    float prob = 1.;
    if (PhotonDiffusion(hitphoton, isc, depth, prob)) return;
    if (PhotonReflection(hitphoton, isc, depth, prob)) return;
    if (PhotonRefraction(hitphoton, isc, depth, prob)) return;
    return;
}

bool PPMEngine::Render() {

	Point3 eye(0, 10, 5);
	Point3 des(0, -5, 0);
	Vector3 dir = Normalize(des - eye);
	Vector3 up = Normalize(Vector3(0, 1, -dir.y / dir.z));
	camera = new Camera(eye, dir, up, width, height);
	for(int i = 0; i < width; i++) {
		for(int j = 0; j < height; j++) {
            Raytrace(Ray(eye, camera -> Emit(i, j)), 0, 1.0f, Vector3(1, 1, 1), i * height + j);
        }
    }
    tree = new KDTree(hps);

    int count = 0;

    while (true) {
        vector<Photon*> photons = scene -> EmitPhotons(PHOTON_COUNT);
        count += PHOTON_COUNT;
        int tot = 0;
        for(auto photon : photons) {
            Photontrace(*photon, 0);
            tot ++;
            cout << tot << endl;
        }

        camera -> clear();

        for(auto hp: hps) {
            Color col = hp -> color / (PI * hp -> R2 * count);
            camera -> AddColor(hp -> id / height, hp -> id % height, col);
        }
        camera -> print();
        for(int i = 0; i < photons.size(); i++)
            delete photons[i];
        photons.clear();
    }

	delete camera;
	return true;
}

Engine* CreatePPMEngine(Scene *scene, int width, int height) {
    return new PPMEngine(scene, width, height);
}

};
