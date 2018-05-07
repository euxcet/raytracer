#include "light.h"

namespace Raytracer {

double PointLight::CalcShade(const Scene *scene, const Point3 &p) {
    double dist = (origin - p).Length();
    Intersection isc;
    Vector3 T = Normalize(origin - p);
    return scene -> Intersect(Ray(p + T * EPS, T, dist), &isc) == 0;
}

vector<Photon*> PointLight::EmitPhotons(int pcount) const {
    double x, y, z;
    vector<Photon*> photons;
    Vector3 to = Normalize(focus - origin);
    for(int i = 0; i < pcount; i++) {
        do {
            x = RAND() * 2 - 1;
            y = RAND() * 2 - 1;
            z = RAND() * 2. - 1;
        } while (x*x + y*y + z*z >= 1 || x*x + y*y + z*z < EPS || Dot(to, Normalize(Vector3(x, y, z))) < scope);
        Photon *photon = new Photon(origin, Normalize(Vector3(x, y, z)), color * power * PI * 4.);
        photons.push_back(photon);
    }
    return photons;
}

double AreaLight::CalcShade(const Scene *scene, const Point3 &p) {
    int shade = 0;
    for(int i = -1; i <= 1; i++) {
        for(int j = -1; j <= 1; j++) {
            for(int t = 0; t < LIGHT_SAMPLE; t++) {
                Intersection isc;
                float rx = (RAND() + i) / 2.;
                float ry = (RAND() + j) / 2.;
                float dist = (origin - p + dx * rx + dy * ry).Length();
                Vector3 T = Normalize(origin - p + dx * rx + dy * ry);
                if (scene -> Intersect(Ray(p + T * EPS, T, dist), &isc) == 0)
                    shade ++;
            }
        }
    }
    return shade / (9. * LIGHT_SAMPLE);
}

vector<Photon*> AreaLight::EmitPhotons(int pcount) const {
    double x, y, z;
    double rx, ry;
    vector<Photon*> photons;
    Vector3 to = Normalize(focus - origin);
    for(int i = 0; i < pcount; i++) {
        do {
            x = RAND() * 2 - 1;
            y = RAND() * 2 - 1;
            z = RAND() * 2 - 1;
        } while (x*x + y*y + z*z >= 1 || x*x + y*y + z*z < EPS || Dot(to, Normalize(Vector3(x, y, z))) < scope);
        rx = RAND() - 0.5;
        ry = RAND() - 0.5;
        Photon *photon = new Photon(origin + dx * rx + dy * ry, Normalize(Vector3(x, y, z)), color * power * PI * 4.);
        photons.push_back(photon);
    }
    return photons;
}

bool AreaLight::Intersect(const Ray &ray) const {
    return false;
}

}
