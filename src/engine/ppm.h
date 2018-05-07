#ifndef ENGINE_PPM_H
#define ENGINE_PPM_H

#include "raytracer.h"
#include "engine.h"

namespace Raytracer {

#define PHOTON_DEPTH 6
#define HITRADIUS 0.1
#define ALPHA 0.8

class HitPoint {
public:
    HitPoint() {}
    Vector3 weight;
    Ray ray;
    Intersection isc;
    int id;
    int tot;
    Color color;
    double R2;
};
class KDTNode {
public:
    KDTNode() { l = r = NULL; }
    ~KDTNode() {
        if (l != NULL) delete l;
        if (r != NULL) delete r;
    }
    KDTNode *l, *r;
    HitPoint* hp;
    int dim;
};

static bool cmpX(HitPoint *a, HitPoint *b) { return a -> isc.p[0] < b -> isc.p[0]; }
static bool cmpY(HitPoint *a, HitPoint *b) { return a -> isc.p[1] < b -> isc.p[1]; }
static bool cmpZ(HitPoint *a, HitPoint *b) { return a -> isc.p[2] < b -> isc.p[2]; }


class KDTree {
public:
    KDTree(const vector<HitPoint*> &vhps);
    ~KDTree() { if (root != NULL) delete root; }
    KDTNode* BuildTree(int u, int v);
    void Update(KDTNode *node, const Photon &photon);

    KDTNode* root;
    HitPoint** hps;
    int hpcount;
    float curR2;
};


class PPMEngine : public Engine {
public:
    PPMEngine(Scene* scene, Camera* camera, int width, int height, int p)
        : Engine(scene, camera, width, height), PHOTON_COUNT(p) { }

	void Raytrace(const Ray& ray, int depth, float index, Vector3 weight, int id);
	void SpawnReflectionRay(const Ray &ray, Intersection isc, int depth, float index, Vector3 weight, int id);
	void SpawnRefractionRay(const Ray &ray, Intersection isc, int depth, float index, Vector3 weight, int id);

    void Photontrace(const Photon &photon, int depth, float index);
    bool PhotonDiffusion(const Photon &photon, const Intersection &isc, int depth, float index, float &prob);
    bool PhotonReflection(const Photon &photon, const Intersection &isc, int depth, float index, float &prob);
    bool PhotonRefraction(const Photon &photon, const Intersection &isc, int depth, float index, float &prob);

	bool Render();

private:
    int PHOTON_COUNT;
    vector<HitPoint*> hps;
    KDTree *tree;
};

Engine* CreatePPMEngine(Scene *scene, Camera *camera, int width, int height, int p);

};

#endif
