#ifndef SHAPE_BEZIER_H
#define SHAPE_BEZIER_H

#include "shape.h"

namespace Raytracer {

class Bezier3 : public Shape {
public:
    Bezier3(const char* file, Transform transform)
        : Shape(transform) {
            controls = new Point3[16 * 32];
            ifstream fin(file);
            int n, m;
            fin >> patchs;
            float x, y, z;
            for(int p = 0; p < patchs; p++) {
                fin >> n >> m;
                Point3 minp = Point3(INF, INF, INF);
                Point3 maxp = Point3(-INF, -INF, -INF);
                for(int i = 0; i <= n; i++)
                    for(int j = 0; j <= m; j++) {
                        fin >> x >> y >> z;
                        controls[p * 16 + i * 4 + j] = transform(Point3(x, y, z));
                        minp = Min(minp, transform(Point3(x, y, z)));
                        maxp = Max(maxp, transform(Point3(x, y, z)));
                    }
                aabb.push_back(AABB(minp, maxp - minp));
            }
        }
    bool Intersect(const Ray& ray, Intersection *isc) const;
    Normal3 GetNormal(const Point3& pos) const { return Normal3(); }
    AABB GetAABB() const { return AABB(); }
    pair<float, float> Coordinate(const Point3& pos) const { return make_pair(0, 0); }

    Vector3 DFDU(float u, float v, Point3* controls) const;
    Vector3 DFDV(float u, float v, Point3* controls) const;
    Vector3 F(const Ray &ray, const Vector3 &x, Point3* controls) const;

private:
    Point3* controls;
    int patchs;
    vector<AABB> aabb;
};

Shape* CreateBezier3Shape(const char* file, Transform transform);

}


#endif
