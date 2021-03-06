#ifndef SCENE_H
#define SCENE_H

#include "raytracer.h"
#include "geometry.h"
#include "primitive.h"
#include "intersection.h"
#include "light.h"

namespace Raytracer {

class Scene {
public:
	Scene(Primitive* aggregate, const vector<Light*> &lights,
			const Color &background = Color())
		: aggregate(aggregate), lights(lights), background(background) {}

	Color Le(const Ray &ray, Intersection isc) const;
	bool Intersect(const Ray &ray, Intersection *isc) const;
	float CalcShade(Point3 o, Point3 p) const;
	vector<Photon*> EmitPhotons(int pcount) const;

private:
	Primitive* aggregate;
	vector<Light*> lights;
	Color background;
};

}

#endif
