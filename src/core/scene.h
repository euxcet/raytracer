#ifndef SCENE_H
#define SCENE_H

#include "raytracer.h"
#include "common.h"
#include "primitive.h"

namespace Raytracer {

class Scene {
public:
	Scene(shared_ptr<Primitive> aggregate) : aggregate(aggregate) {}
	int Intersect(const Ray &ray, float &dist) const;

private:
	shared_ptr<Primitive> aggregate;
};

};

#endif
