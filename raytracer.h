#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "common.h"
#include "scene.h"
#include "ray.h"

namespace Raytracer {


class Scene;
class Primitive;

class Engine {
public:
	Engine() { scene = new Scene(); }
	~Engine() { delete scene; }
	Scene* GetScene() { return scene; }
	void SetTarget(int _width, int _height) {
		width = _width;
		height = _height;
	}

	Primitive* Raytrace(const Ray& ray, Color& acc, int depth, float index, float& dist);
	void Init();
	bool Render();


protected:
	Scene* scene;
	int width;
	int height;

	ofstream out;



};

};

#endif
