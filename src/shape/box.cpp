#include "shape/box.h"

namespace Raytracer {

int Box::Intersect(const Ray& ray, float& dist ) const
{
    float dis[6];
    Vector3 ip[6], d = ray.GetDirection(), o = ray.GetOrigin();
    bool retval = MISS;
    for ( int i = 0; i < 6; i++ ) dis[i] = -1;
    Vector3 v1 = GetPos(), v2 = GetPos() + GetSize();

    if (d.x)
    {
        float rc = 1.0f / d.x;
        dis[0] = (v1.x - o.x) * rc;
        dis[3] = (v2.x - o.x) * rc;
    }
    if (d.y)
    {
        float rc = 1.0f / d.y;
        dis[1] = (v1.y - o.y) * rc;
        dis[4] = (v2.y - o.y) * rc;
    }
    if (d.z)
    {
        float rc = 1.0f / d.z;
        dis[2] = (v1.z - o.z) * rc;
        dis[5] = (v2.z - o.z) * rc;
    }
    for ( int i = 0; i < 6; i++ ) if (dis[i] > 0)
    {
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

Normal3 Box::GetNormal(const Point3& pos ) const
{
	float dist[6];
	dist[0] = (float)fabs(GetSize().x - GetPos().x );
	dist[1] = (float)fabs(GetSize().x + GetSize().x - GetPos().x );
	dist[2] = (float)fabs(GetSize().y - GetPos().y );
	dist[3] = (float)fabs(GetSize().y + GetSize().y - GetPos().y );
	dist[4] = (float)fabs(GetSize().z - GetPos().z );
	dist[5] = (float)fabs(GetSize().z + GetSize().z - GetPos().z );
	int best = 0;
	float bdist = dist[0];
	for ( int i = 1 ; i < 6; i++ ) if (dist[i] < bdist)
	{
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
