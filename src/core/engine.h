#ifndef ENGINE_H
#define ENGINE_H

#include "raytracer.h"
#include "primitive.h"
#include "scene.h"
#include "common.h"

namespace Raytracer {

class Engine {
public:
	Engine() { scene = new Scene(); }
	~Engine() { delete scene; }
	Scene* GetScene() { return scene; }
	void SetTarget(int _width, int _height) {
		width = _width;
		height = _height;
	}

	int FindNearest(const Ray& ray, float& dist, Primitive*& prim);
	Primitive* Raytrace(const Ray& ray, Color& acc, int depth, float index, float& dist);

	float CalcShade(Primitive* light, Vector3 pi, Vector3& dir);

	bool Render();


protected:
	Scene* scene;
	int width;
	int height;

	ofstream out;
};

}

#endif
