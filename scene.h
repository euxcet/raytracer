#ifndef SCENE_H
#define SCENE_H

#include "ray.h"
#include "primitive.h"

namespace Raytracer {

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
