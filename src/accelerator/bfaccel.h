#include "raytracer.h"
#include "primitive.h"

namespace raytracer {

class BFAccel : public Aggregate {
public:
    BFAccel(const vector<Primitive*> &primitives)
        : primitives(primitives) {}

    int Intersect(const Ray& ray, ) const;

private:
    vector<Primitive*> primitives;

};


Aggregate* CreateBFAccelerator(vector<Primitive*> primitives) {
    return new BFAccel(primitives);
}
