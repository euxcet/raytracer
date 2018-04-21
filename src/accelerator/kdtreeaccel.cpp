#include "accelerator/kdtreeaccel.h"

namespace Raytracer {

    KDTreeAccel::KDTreeAccel(const vector<Mesh*> &meshs,
                             const vector<Primitive*> &others)
        : others(others) {
        AABB scene(Point3(INF, INF, INF), Vector3(-2 * INF, -2 * INF, -2 * INF));
        for(auto mesh : meshs) {
            for(auto triangle : mesh -> GetTriangles())
                triangles.push_back(triangle);
        }
        for(auto triangle : triangles) {
            scene = scene.Combine(triangle -> GetShape() -> GetAABB());
        }
        root = BuildTree(scene, triangles);
    }

    KDTreeNode* KDTreeAccel::BuildTree(const AABB &scene,
                                       const vector<Primitive*> &tris) {
        KDTreeNode *node = new KDTreeNode();
        node -> box = scene;
        if (tris.size() < 30) {
            node -> prims = tris;
            node -> leaf = true;
            return node;
        }
        int dim = rand() % 3;
        dim = 0;
        if (scene.size[1] > scene.size[0]) dim = 1;
        if (scene.size[2] > scene.size[0] && scene.size[2] > scene.size[1]) dim = 2;
        AABB lab = scene.CutLeft(dim);
        AABB rab = scene.CutRight(dim);

        vector<Primitive*> ltris;
        vector<Primitive*> rtris;
        for(auto triangle : tris) {
            if (lab.Intersect(triangle -> GetShape() -> GetAABB())) {
                ltris.push_back(triangle);
            }
            if (rab.Intersect(triangle -> GetShape() -> GetAABB())) {
                rtris.push_back(triangle);
            }
        }
        node -> l = BuildTree(lab, ltris);
        node -> r = BuildTree(rab, rtris);
        return node;
    }

    bool KDTreeAccel::Intersect(const Ray& ray, Intersection *isc) const {
        float tmax = ray.tmax;
        bool res = false;
        for (auto primitive : others) {
            if (primitive -> Intersect(Ray(ray.origin, ray.direction, tmax), isc)) {
                tmax = isc -> dist;
                res = true;
            }
        }
        if (Traverse(root, ray, isc, tmax)) res = true;
        return res;
    }

    bool KDTreeAccel::Traverse(KDTreeNode *node, const Ray& ray,
                               Intersection *isc, float &tmax) const {
        if (node -> IsLeaf()) {
            bool res = false;
            for(auto primitive : node -> prims) {
                if (primitive -> Intersect(Ray(ray.origin, ray.direction, tmax), isc)) {
                    tmax = isc -> dist;
                    res = true;
                }
            }
            return res;
        }
        if (node -> l == NULL) return Traverse(node -> r, ray, isc, tmax);
        if (node -> r == NULL) return Traverse(node -> l, ray, isc, tmax);
        float lhit0, lhit1;
        float rhit0, rhit1;
        float lres = node -> l -> box.Intersect(ray, lhit0, lhit1);
        float rres = node -> r -> box.Intersect(ray, rhit0, rhit1);
        if (lres == false && rres == false) return false;
        if (lres == false) return Traverse(node -> r, ray, isc, tmax);
        if (rres == false) return Traverse(node -> l, ray, isc, tmax);
        bool res = false;
        if (lhit0 < rhit0) {
            res = Traverse(node -> l, ray, isc, tmax);
            if (res == false) return Traverse(node -> r, ray, isc, tmax);
            if (!node -> l -> box.Contain(isc -> p)) {
                Traverse(node -> r, ray, isc, tmax);
                return true;
            }
        }
        else {
            res = Traverse(node -> r, ray, isc, tmax);
            if (res == false) return Traverse(node -> l, ray, isc, tmax);
            if (!node -> r -> box.Contain(isc -> p)) {
                Traverse(node -> l, ray, isc, tmax);
                return true;
            }
        }
        return res;
    }


    Aggregate* CreateKDTreeAccelerator(vector<Mesh*> meshs,
                                       vector<Primitive*> others) {
        return new KDTreeAccel(meshs, others);
    }
}
