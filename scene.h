#ifndef SCENE_H
#define SCENE_H

namespace Raytracer {

#define HIT 1
#define MISS 0
#define INPRIM -1

class Material {
public:
	Material() : color(Color(0.2f, 0.2f, 0.2f)), refl(0), diff(0.2f) {}
	void SetColor(Color& _color) { color = _color; }
	void SetDiffuse(float _diff) { diff = _diff; }
	void SetReflection(float _refl) { refl = _refl; }
	Color GetColor() { return color; }
	float GetDiffuse() { return diff; }
	float GetReflection() { return refl; }
	float GetSpecular() { return 1.0f - diff; } // diffuse + specular = 1

private:
	Color color;
	float diff;
	float refl;
};

class Primitive {
public:
	enum {
		SPHERE = 1,
		PLANE
	};
	Primitive() : name(0), light(false) {}

	Material* GetMaterial() { return &material; }
	void SetMaterial(Material& _m) { material = m; }
	virtual int GetType() = 0;
	virtual int Intersect(Ray& ray, float& dist) = 0;
	virtual vec3 GetNormal(vec3& pos) = 0;
	virtual Color GetColor(vec3) { return material.GetColor(); }
	virtual void Light( bool _l) { light = _l; } // TODO: ?GetLight
	bool IsLight() { return light; }
	void SetName(char* _n) {
		delete name;
		name = new char[strlen(_n) + 1];
		strcpy(name, _n);
	}

protected:
	Material material;
	char* name;
	bool light;
};

class Sphere : public Primitive {
public:
	int GetType() { return SPHERE; }
	Sphere(vec3& _center, float _radius) :
		center(_center), radius(radius) {}
	vec3 GetCenter() { return center; }
	float getSqrRadius() { return radius * radius; }
	int Intersect(Ray& ray, float& dist);
	vec3 GetNormal(vec3 pos) { return (pos - center) / radius; }

private:
	vec3 center;
	float radius;
};

class PlanePrim : public Primitive {
public:
	int GetType() { return PLANE; }
	PlanePrim(vec3& normal, float d) : plane( Plane(normal, d) ) {}
	vec3& GetNormal() { return plane.N; }
	int Intersect(Ray& ray, float& dist);
	vec3 GetNormal(vec3& pos) { return plane.N; }
private:
	Plane plane;
};

class Scene {
public:
	Scene() : primcnt(0), primitive(0) {}
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
