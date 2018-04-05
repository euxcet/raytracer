#include "shape/plane.h"


namespace Raytracer {

    bool Plane::Intersect(const Ray& ray, Intersection *isc) const {
        float dot = Dot(normal, ray.GetDirection());
        if (dot > EPS || dot < -EPS) {
            float dis = -(Dot(normal, Vector3(ray.GetOrigin())) + dist) / dot;
            if (dis > 0) {
                if (dis < ray.tmax) {
					isc -> dist = dis;
					isc -> hit = 1;
					isc -> p = ray(dis);
					isc -> n = normal;
                    return 1;
                }
            }
        }
        return 0;
    }

    pair<float, float> Plane::Coordinate(const Point3& pos) const {
        float x = pos.x / 6;
        float y = pos.y / 6;
        return make_pair(x - floor(x), y - floor(y));
    }

    Plane* CreatePlaneShape(const Normal3 &normal, const Point3 &origin) {
        return new Plane(normal, origin);
    }

}
