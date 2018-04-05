#include "light.h"

namespace Raytracer {

double PointLight::CalcShade(const Scene *scene, const Point3 &p) {
    double dist = (origin - p).Length();
    Intersection isc;
    Vector3 T = Normalize(origin - p);
    return scene -> Intersect(Ray(p + T * EPS, T, dist), &isc) == 0;
}

double AreaLight::CalcShade(const Scene *scene, const Point3 &p) {
    int shade = 0;
    for(int i = -1; i <= 1; i++) {
        for(int j = -1; j <= 1; j++) {
            for(int t = 0; t < LIGHT_SAMPLE; t++) {
                Intersection isc;
                float rx = (rand() % 10000 / 10000. + i) / 2.;
                float ry = (rand() % 10000 / 10000. + j) / 2.;
                float dist = (origin - p + dx * rx + dy * ry).Length();
                Vector3 T = Normalize(origin - p + dx * rx + dy * ry);
                if (scene -> Intersect(Ray(p + T * EPS, T, dist), &isc) == 0)
                    shade ++;
            }
        }
    }
    return shade / (9. * LIGHT_SAMPLE);
}

bool AreaLight::Intersect(const Ray &ray) const {
    return false;
}

}
