#include "raytracer.h"
#include "geometry.h"
#include "primitive.h"

namespace Raytracer {

class KDTreeNode {
public:
    KDTreeNode() { l = r = NULL; box = AABB(); leaf = false; }
    
    bool IsLeaf() const { return leaf; }

    KDTreeNode *l, *r;
    AABB box;
    bool leaf;
    vector<Primitive*> prims;
};

class KDTreeAccel : public Aggregate {
public:
    KDTreeAccel(const vector<Primitive*> &triangles,
                const vector<Primitive*> &others);
    KDTreeNode* BuildTree(const AABB &scene, const vector<Primitive*> tris);
    bool Traverse(KDTreeNode *node, const Ray& ray, Intersection* isc, float &tmax) const;
    bool Intersect(const Ray& ray, Intersection* isc) const;

private:
    vector<Primitive*> triangles;
    vector<Primitive*> others;
    KDTreeNode *root;

};


Aggregate* CreateKDTreeAccelerator(vector<Primitive*> triangles,
                                   vector<Primitive*> others);

}
