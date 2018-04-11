#ifndef ENGINE_PPM_H
#define ENGINE_PPM_H

#include "raytracer.h"
#include "engine.h"

namespace Raytracer {

#define PHOTON_COUNT 1000
#define PHOTON_DEPTH 6
#define COLLECT_RADIUS 0.3
#define ALPHA 0.8

class HitPoint {
public:
    HitPoint() { cnt = 0; }
    Vector3 weight;
    Ray ray;
    Intersection isc;
    int id;
    int cnt;
    int tot;
    Color color;
    double R2;
};
class KDTNode {
public:
    KDTNode() { l = r = NULL; }
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
    void FindNearest(KDTNode *node, Point3 p, float &mdist, float pdist, HitPoint *&hp);
    void Update(KDTNode *node, const Photon &photon);

    KDTNode* root;
    HitPoint** hps;
    int hpcount;
};


class PPMEngine : public Engine {
public:
    PPMEngine(Scene* scene, int width, int height)
        : scene(scene), width(width), height(height) {}

	void Raytrace(const Ray& ray, int depth, float index, Vector3 weight, int id);
	void SpawnReflectionRay(const Ray &ray, Intersection isc, int depth, float index, Vector3 weight, int id);
	void SpawnRefractionRay(const Ray &ray, Intersection isc, int depth, float index, Vector3 weight, int id);

    void Photontrace(const Photon &photon, int depth);
    bool PhotonDiffusion(const Photon &photon, const Intersection &isc, int depth, float &prob);
    bool PhotonReflection(const Photon &photon, const Intersection &isc, int depth, float &prob);
    bool PhotonRefraction(const Photon &photon, const Intersection &isc, int depth, float &prob);

	bool Render();

private:
	Scene* scene;
    vector<HitPoint*> hps;
    KDTree *tree;
	int width;
	int height;
    Camera *camera;
};

Engine* CreatePPMEngine(Scene *scene, int width, int height);

};

#endif
