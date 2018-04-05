#ifndef ENGINE_H
#define ENGINE_H

#include "raytracer.h"
#include "primitive.h"
#include "scene.h"
#include "geometry.h"
#include "camera.h"

namespace Raytracer {

class Engine {
public:
	Engine(Scene* scene, int width, int height)
        : scene(scene), width(width), height(height) {}

	Color Raytrace(const Ray& ray, int depth, float index);
	Color SpawnReflectionRay(const Ray &ray, Intersection isc, int depth, float index);
	Color SpawnRefractionRay(const Ray &ray, Intersection isc, int depth, float index);

	bool Render();


protected:
	Scene* scene;
	int width;
	int height;
	ofstream out;
};

}

#endif
