#include "shape/triangle.h"

namespace Raytracer {

bool Triangle::Intersect(const Ray& ray, Intersection *isc) const {

    float dot = Dot(N, ray.GetDirection());
    if (fabs(dot) < EPS) return false;
    Normal3 n = N;
    float d = Dot(n, Vector3(v0));
    float dist = -(Dot(n, Vector3(ray.GetOrigin())) - d) / dot;
    if (dist < 0 || dist > ray.tmax) return false;
    Point3 p = ray(dist);
    Vector3 edge = v1 - v0;
    Vector3 vp = p - v0;
    if (Dot(n, Cross(edge, vp)) < 0) return false;
    edge = v2 - v1;
    vp = p - v1;
    if (Dot(n, Cross(edge, vp)) < 0) return false;
    edge = v0 - v2;
    vp = p - v2;
    if (Dot(n, Cross(edge, vp)) < 0) return false;

	isc -> dist = dist;
	isc -> hit = 1;
	isc -> p = p;
	isc -> n = GetNormal(isc -> p);
    if (dot > 0) {
        isc -> hit = -1;
        isc -> n = -isc -> n;
    }

    return true;

}

AABB Triangle::GetAABB() const {
    Point3 p = Point3(min(min(v0.x, v1.x), v2.x),
                      min(min(v0.y, v1.y), v2.y),
                      min(min(v0.z, v1.z), v2.z));
    Point3 p2= Point3(max(max(v0.x, v1.x), v2.x),
                      max(max(v0.y, v1.y), v2.y),
                      max(max(v0.z, v1.z), v2.z));
    return AABB(p, p2 - p);
}

pair<float, float> Triangle::Coordinate(const Point3& pos) const {
    if (vt.size() == 0) return make_pair(0, 0);
    Vector3 v0v1 = v1 - v0;
    Vector3 v0v2 = v2 - v0;
    float area = Cross(v0v1, v0v2).Length() / 2;
    Vector3 edge = v2 - v1;
    Vector3 vp = pos - v1;
    float u = Cross(edge, vp).Length() / 2 / area;
    edge = v0 - v2;
    vp = pos - v2;
    float v = Cross(edge, vp).Length() / 2 / area;
    float w = 1 - u - v;
    float rx = vt[0].x * u + vt[1].x * v + vt[2].x * w;
    float ry = vt[0].y * u + vt[1].y * v + vt[2].y * w;
    return make_pair(rx, 1 - ry);
}

Shape* CreateTriangleShape(const Point3 &v0, const Point3 &v1, const Point3 &v2) {
    return new Triangle(v0, v1, v2);
}

Shape* CreateTriangleShape(const Point3 &v0, const Point3 &v1, const Point3 &v2,
                           const vector<Point3> &vt) {
    return new Triangle(v0, v1, v2, vt);
}

}
