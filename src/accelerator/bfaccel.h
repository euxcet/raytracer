#ifndef ACCELERATOR_BFACCEL_H
#define ACCELERATOR_BFACCEL_H

#include "raytracer.h"
#include "primitive.h"

namespace Raytracer {

class BFAccel : public Aggregate {
public:
    BFAccel(const vector<Primitive*> &primitives)
        : primitives(primitives) {}

    bool Intersect(const Ray& ray, Intersection* isc) const;

private:
    vector<Primitive*> primitives;

};


Aggregate* CreateBFAccelerator(vector<Primitive*> primitives);

}

#endif
