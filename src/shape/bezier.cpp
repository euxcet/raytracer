#include "shape/bezier.h"

namespace Raytracer {

Vector3 Bezier3::DFDU(float u, float v, Point3* controls) const {
    Vector3 res;
    float C[4]; C[0] = 1; C[1] = 3; C[2] = 3; C[3] = 1;
    float U[4]; U[0] = -3+6*u-3*u*u; U[1] = 1-4*u+3*u*u; U[2] = 2*u-3*u*u; U[3] = 3*u*u;
    float V[4]; V[0] = (1-v)*(1-v)*(1-v); V[1] = v*(1-v)*(1-v); V[2] = v*v*(1-v); V[3] = v*v*v;
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++) {
            res += Vector3(-1 * (U[i] * V[j] * C[i] * C[j]) * controls[i * 4 + j]);
        }
    return res;
}

Vector3 Bezier3::DFDV(float u, float v, Point3* controls) const {
    Vector3 res;
    float C[4]; C[0] = 1; C[1] = 3; C[2] = 3; C[3] = 1;
    float U[4]; U[0] = (1-u)*(1-u)*(1-u); U[1] = u*(1-u)*(1-u); U[2] = u*u*(1-u); U[3] = u*u*u;
    float V[4]; V[0] = -3+6*v-3*v*v; V[1] = 1-4*v+3*v*v; V[2] = 2*v-3*v*v; V[3] = 3*v*v;
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++) {
            res += Vector3(-1 * (U[i] * V[j] * C[i] * C[j]) * controls[i * 4 + j]);
        }
    return res;
}

Vector3 Bezier3::F(const Ray &ray, const Vector3 &x, Point3* controls) const {
    Vector3 res = Vector3(ray.origin + ray.direction * x.x);
    float u = x.y;
    float v = x.z;
    float C[4]; C[0] = 1; C[1] = 3; C[2] = 3; C[3] = 1;
    float U[4]; U[0] = (1-u)*(1-u)*(1-u); U[1] = u*(1-u)*(1-u); U[2] = u*u*(1-u); U[3] = u*u*u;
    float V[4]; V[0] = (1-v)*(1-v)*(1-v); V[1] = v*(1-v)*(1-v); V[2] = v*v*(1-v); V[3] = v*v*v;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            res += Vector3(-1 * (U[i] * V[j] * C[i] * C[j]) * controls[i * 4 + j]);
        }
    }
    return res;
}

bool Bezier3::Intersect(const Ray& ray, Intersection *isc) const {
    float mdist = ray.tmax;
    bool hit = false;
    for(int _ = 0; _ < patchs; _++) {

        float hit0, hit1;
        if (!aabb[_].Expand().Intersect(ray, hit0, hit1)) continue;

        for(int ti = 0; ti < 5; ti ++) {
            Vector3 res = Vector3(hit0 + RAND() * (hit1 - hit0), RAND(), RAND());
            bool hitthis = false;
            for(int i = 0; i < 5; i++) {
                float u = res.y;
                float v = res.z;
                Vector3 dfdu = DFDU(u, v, controls + _ * 16);
                Vector3 dfdv = DFDV(u, v, controls + _ * 16);
                Vector3 dir = ray.direction;
                Matrix3 df = Matrix3(dir.x, dfdu.x, dfdv.x,
                                     dir.y, dfdu.y, dfdv.y,
                                     dir.z, dfdu.z, dfdv.z);
                Vector3 f = F(ray, res, controls + _ * 16);
                if (res.x > 0 && u >= -EPS && u <= 1 + EPS && v >= -EPS && v <= 1 + EPS && f.Length() < 0.0001 && res.x < mdist) {
                    mdist = res.x;
                    isc -> hit = 1;
                    isc -> dist = res.x;
                    isc -> p = ray(res.x);
                    isc -> n = Normalize(Normal3(Cross(dfdu, dfdv)));
                    hit = true;
                    hitthis = true;
                    break;
                }
                res = res - Inverse(df) * f;
            }
            if (hitthis) break;
        }
    }
    return hit;
}

Shape* CreateBezier3Shape(const char* file, Transform transform) {
    return new Bezier3(file, transform);
}

}
