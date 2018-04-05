#include "light.h"

namespace Raytracer {

double PointLight::CalcShade(const Scene *scene, const Point3 &p) {
    double dist = (origin - p).Length();
    Intersection isc;
    Vector3 T = Normalize(origin - p);
    return scene -> Intersect(Ray(p + T * EPS, T, dist), &isc) == 0;
}

}
