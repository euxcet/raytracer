#ifndef SCENE_H
#define SCENE_H

#include "ray.h"

namespace Raytracer {

#define HIT 1
#define MISS 0
#define INPRIM -1

class Material {
public:
	Material() : color(Color(0.2f, 0.2f, 0.2f)), refl(0), diff(0.2f), spec(0.8f), rindex(1.5f) {}

	void SetColor(const Color& _color) { color = _color; }
	void SetDiffuse(float _diff) { diff = _diff; }
	void SetReflection(float _refl) { refl = _refl; }
	void SetRefraction(float _refr) { refr = _refr; }
	void SetRefrIndex(float _rindex) { rindex = _rindex; }
	void SetSpecular(float _spec) { spec = _spec; }

	Color GetColor() { return color; }

	float GetDiffuse() { return diff; }
	float GetReflection() { return refl; }
	float GetRefraction() { return refr; }
	float GetRefrIndex() { return rindex; }
	float GetSpecular() { return spec; }

private:
	Color color;
	float diff, spec;
	float refl, refr;
	float rindex;
};

class Primitive {
public:
	enum {
		SPHERE = 1,
		PLANE
	};
	Primitive() : light(false) {name = "";}

	Material* GetMaterial() { return &material; }
	void SetMaterial(Material& _m) { material = _m; }
	virtual int GetType() = 0;
	virtual int Intersect(const Ray& ray, float& dist) = 0;
	virtual vec3 GetNormal(const vec3& pos) = 0;
	virtual Color GetColor(vec3) { return material.GetColor(); }
	virtual void Light( bool _l) { light = _l; } // TODO: ?GetLight
	bool IsLight() { return light; }
	void SetName(string _n) { name = _n; }

protected:
	Material material;
	string name;
	bool light;
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

class Scene {
public:
	Scene() : primcnt(0), primitive(0) { Init(); }
	~Scene() { delete primitive; }
	void Init();
	int GetPrimcnt() { return primcnt; }
	Primitive* GetPrimitive(int id) { return primitive[id]; }

private:
	int primcnt;
	Primitive** primitive;
};

};

#endif
