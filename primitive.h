#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "common.h"
#include "ray.h"
#include "material.h"

namespace Raytracer {

class Primitive {
public:
	enum {
		SPHERE = 1,
		PLANE = 2,
		BOX = 3,
		TRIANGLE
	};
	Primitive() : light(false) {name = "";}

	Material* GetMaterial() { return &material; }
	void SetMaterial(Material& _m) { material = _m; }
	virtual int GetType() = 0;
	virtual int Intersect(const Ray& ray, float& dist) = 0;
	virtual vec3 GetNormal(const vec3& pos) = 0;
//	virtual aabb GetAABB() = 0;

	virtual Color GetColor(vec3) { return material.GetColor(); }
	virtual void Light( bool _l) { light = _l; } // TODO: ?GetLight
	virtual void setDebug(bool d) { DEBUG = d; }
	virtual void print() {}
	bool IsLight() { return light; }
	void SetName(string _n) { name = _n; }

protected:
	Material material;
	string name;
	bool light;
	bool DEBUG;
};

class Sphere : public Primitive {
public:
	int GetType() { return SPHERE; }
	Sphere(const vec3& _center, float _radius) :
		center(_center), radius(_radius) {}
	vec3 GetCenter() { return center; }
	float GetSqrRadius() { return radius * radius; }
	int Intersect(const Ray& ray, float& dist);
	vec3 GetNormal(const vec3& pos) { return (pos - center) / radius; }


	void check(vec3 p) {
		double dis = (p - center).Length();
		if (dis < radius) cout << " Inside the sphere\n";
		if (dis == radius) cout << " On the sphere\n";
		if (dis > radius) cout << " Outside the sphere\n";

	}

	void print() {
		puts("---SPHERE---");
		printf("CENTER: %lf %lf %lf\n", center.x, center.y, center.z);
		printf("RADIUS: %lf\n", radius);
		puts("------------");


	}

private:
	vec3 center;
	float radius;
};

class PlanePrim : public Primitive {
public:
	int GetType() { return PLANE; }
	PlanePrim(const vec3& normal, float d) : plane( Plane(normal, d) ) {}
	vec3& GetNormal() { return plane.N; }
	int Intersect(const Ray& ray, float& dist);
	vec3 GetNormal(const vec3& pos) { return plane.N; }
private:
	Plane plane;
};

class Box : public Primitive {
public:
	int GetType() { return BOX; }
	Box() : box(vec3(0, 0, 0), vec3(0, 0, 0)) {}
	Box(const aabb& _box) : box(_box) {}

	bool IntersectBox(const aabb& _box) { return box.Intersect(_box); }
	bool Contain(const vec3& p) { return box.Contain(p); }


	int Intersect(const Ray& ray, float& dist);
	vec3 GetNormal(const vec3& pos);
	vec3 GetPos() { return box.GetPos(); }
	vec3 GetSize() { return box.GetSize(); }
	
	aabb GetAABB() { return box; }

protected:
	aabb box;
};


}

#endif
