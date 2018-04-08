#include "shape/box.h"

namespace Raytracer {

bool Box::Intersect(const Ray& ray, Intersection *isc) const {
    float dis[6];
    Point3 ip[6];
    Point3 o = ray.GetOrigin();
    Vector3 d = ray.GetDirection();
    for(int i = 0; i < 6; i++) dis[i] = -1;
    Point3 v1 = GetPos();
    Point3 v2 = GetPos() + GetSize();

    if (d.x) {
        float rc = 1.0f / d.x;
        dis[0] = (v1.x - o.x) * rc;
        dis[3] = (v2.x - o.x) * rc;
    }
    if (d.y) {
        float rc = 1.0f / d.y;
        dis[1] = (v1.y - o.y) * rc;
        dis[4] = (v2.y - o.y) * rc;
    }
    if (d.z) {
        float rc = 1.0f / d.z;
        dis[2] = (v1.z - o.z) * rc;
        dis[5] = (v2.z - o.z) * rc;
    }
    double dist = ray.tmax;
    for(int i = 0; i < 6; i++)
        if (dist[i] > 0) {
            ip[i] = o + dis[i] * d;
            if ((ip[i].x > (v1.x - EPS)) && (ip[i].x < (v2.x + EPS)) &&
            (ip[i].y > (v1.y - EPS)) && (ip[i].y < (v2.y + EPS)) &&
            (ip[i].z > (v1.z - EPS)) && (ip[i].z < (v2.z + EPS)))
            {
                if (dis[i] < dist)
                {
                    dist = dis[i];
                    retval = HIT;
                }
            }
        }
    return retval;
}

Normal3 Box::GetNormal(const Point3& p) const
{
	float dist[6];
    Point3 pos = GetPos();
    Vector3 size = GetSize();
	dist[0] = (float)fabs(size.x - pos.x );
	dist[1] = (float)fabs(size.x + size.x - pos.x );
	dist[2] = (float)fabs(size.y - pos.y );
	dist[3] = (float)fabs(size.y + size.y - pos.y );
	dist[4] = (float)fabs(size.z - pos.z );
	dist[5] = (float)fabs(size.z + size.z - pos.z );
	int best = 0;
	float bdist = dist[0];
    for(int i = 1; i < 6; i++)
        if (dist[i] < bdist) {
            bdist = dist[i];
            best = i;
        }
	if (best == 0) return Normal3( -1, 0, 0 );
	else if (best == 1) return Normal3( 1, 0, 0 );
	else if (best == 2) return Normal3( 0, -1, 0 );
	else if (best == 3)  return Normal3( 0, 1, 0 );
	else if (best == 4) return Normal3( 0, 0, -1 );
	else return Normal3( 0, 0, 1 );
}

}
