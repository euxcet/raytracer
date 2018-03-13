#include "primitive.h"

namespace Raytracer {


int Sphere::Intersect(const Ray& ray, float& dist) {
	vec3 v = ray.GetOrigin() - center;
	float b = -v.Dot(ray.GetDirection());
	float det = (b * b) - v.Dot(v) + radius * radius;
	int res = MISS;
	if (det > 0) {
		det = sqrtf(det);
		float i1 = b - det;
		float i2 = b + det;
		if (i2 > 0) {
			if (i1 < 0) {
				if (i2 < dist) {
					dist = i2;
					res = INPRIM;
				}
			}
			else if (i1 < dist) {
				dist = i1;
				res = HIT;
			}
		}
	}
	return res;
}

int PlanePrim::Intersect(const Ray& ray, float& dist) {
	float d = plane.N.Dot(ray.GetDirection());
	if (d != 0) { // TODO: eps
		float dis = -(plane.N.Dot(ray.GetOrigin()) + plane.D) / d;
		if (dis > 0) {
			if (dis < dist) {
				dist = dis;
				return HIT;
			}
		}
	}
	return MISS;
}

int Box::Intersect(const Ray& ray, float& dist )
{
	float dis[6];
	vec3 ip[6], d = ray.GetDirection(), o = ray.GetOrigin();
	bool retval = MISS;
	for ( int i = 0; i < 6; i++ ) dis[i] = -1;
	vec3 v1 = GetPos(), v2 = GetPos() + GetSize();

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

vec3 Box::GetNormal(const vec3& pos )
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
	if (best == 0) return vec3( -1, 0, 0 );
	else if (best == 1) return vec3( 1, 0, 0 );
	else if (best == 2) return vec3( 0, -1, 0 );
	else if (best == 3)  return vec3( 0, 1, 0 );
	else if (best == 4) return vec3( 0, 0, -1 );
	else return vec3( 0, 0, 1 );
}


}

