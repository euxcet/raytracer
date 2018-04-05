#include "accelerator/bfaccel.h"

namespace Raytracer {

    bool BFAccel::Intersect(const Ray& ray, Intersection *isc) const {
        float tmax = ray.tmax;
        bool res = false;
        for (auto primitive : primitives) {
            if (primitive -> Intersect(Ray(ray.origin, ray.direction, tmax), isc)) {
                tmax = isc -> dist;
                res = true;
            }
        }
        return res;
    }


    Aggregate* CreateBFAccelerator(vector<Primitive*> primitives) {
        return new BFAccel(primitives);
    }
}
