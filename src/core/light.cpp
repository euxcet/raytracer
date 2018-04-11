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
    for(int i = 0; i < pcount; i++) {
        do {
            //TODO : 10000 is too small
            x = rand() % 20000 / 10000. - 1;
            y = rand() % 20000 / 10000. - 1;
            z = rand() % 20000 / 10000. - 1;
        } while (x*x + y*y + z*z >= 1 || x*x + y*y + z*z < EPS);
        Photon *photon = new Photon(origin, Normalize(Vector3(x, y, z)), Vector3(80, 80, 80) * PI * 4.);
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

vector<Photon*> AreaLight::EmitPhotons(int pcount) const {
    double x, y, z;
    double rx, ry;
    vector<Photon*> photons;
    for(int i = 0; i < pcount; i++) {
        do {
            //TODO : 10000 is too small
            x = rand() % 20000 / 10000. - 1;
            y = rand() % 20000 / 10000. - 1;
            z = rand() % 20000 / 10000. - 1;
        } while (x*x + y*y + z*z >= 1 || x*x + y*y + z*z < EPS);
        rx = rand() % 10000 / 10000. - 0.5;
        ry = rand() % 10000 / 10000. - 0.5;
        Photon *photon = new Photon(origin + dx * rx + dy * ry, Normalize(Vector3(x, y, z)), Color(2500, 2500, 2500));
        photons.push_back(photon);
    }
    return photons;
}

bool AreaLight::Intersect(const Ray &ray) const {
    return false;
}

}
