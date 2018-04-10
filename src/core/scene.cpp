#include "scene.h"

namespace Raytracer {

Color Scene::Le(const Ray &ray, Intersection isc) const {
    Color color = isc.primitive -> GetMaterial() ->
					GetColor(isc.primitive -> GetShape() -> Coordinate(isc.p));
    double diff = isc.primitive -> GetMaterial() -> GetDiffuse();
    double spec = isc.primitive -> GetMaterial() -> GetSpecular();
    // No Texture
    Color res = color * background * diff;
    if (diff < EPS && spec < EPS) return res;
    for(auto light: lights) {
        double shade = light -> CalcShade(this, isc.p);
        Vector3 R = Normalize(light -> GetOrigin() - isc.p);
        double dot = Dot(R, isc.n);
        if (dot > EPS) {
            res += color * light -> GetColor() * diff * dot * shade;
            res += color * light -> GetColor() * spec * pow(dot, 20) * shade;
        }
    }
    return res;
}

bool Scene::Intersect(const Ray &ray, Intersection *isc) const {
    return aggregate -> Intersect(ray, isc);
}

};
