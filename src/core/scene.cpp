#include "scene.h"

namespace Raytracer {

float Scene::CalcShade(Point3 o, Point3 p) const {
    double dist = (o - p).Length();
    Intersection isc;
    return Intersect(Ray(o, p - o, dist), &isc) == 0;
}

Color Scene::Le(const Ray &ray, Intersection isc) const {
    Color color = isc.primitive -> GetMaterial() -> GetColor();
    double diff = isc.primitive -> GetMaterial() -> GetDiffuse();
    double spec = isc.primitive -> GetMaterial() -> GetSpecular();
    cout << "color: " << color << endl;
    cout << diff << " " << spec << endl;
    // No Texture
    Color res = Color();
    for(auto light: lights) {
        double shade = CalcShade(light -> GetOrigin(), isc.p);
        shade = 1.;
        if (shade < EPS) continue;
        Vector3 R = Normalize(light -> GetOrigin() - isc.p);
        double dot = Dot(R, isc.n);
        cout << R << " " << isc.n << endl;
        if (dot > EPS) {
            res += color * light -> GetColor() * diff * dot * shade;
            res += color * light -> GetColor() * spec * pow(dot, 20) * shade;
        }
    }
    cout << res << endl;
    puts("");
    return res;
}

bool Scene::Intersect(const Ray &ray, Intersection *isc) const {
    return aggregate -> Intersect(ray, isc);
}

};
