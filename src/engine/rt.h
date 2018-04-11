#ifndef ENGINE_RT_H
#define ENGINE_RT_H

#include "raytracer.h"
#include "engine.h"

namespace Raytracer {

class RaytracerEngine : public Engine {
public:
    RaytracerEngine(Scene* scene, int width, int height)
        : scene(scene), width(width), height(height) {}

	Color Raytrace(const Ray& ray, int depth, float index);
	Color SpawnReflectionRay(const Ray &ray, Intersection isc, int depth, float index);
	Color SpawnRefractionRay(const Ray &ray, Intersection isc, int depth, float index);
	bool Render();


private:
	Scene* scene;
	int width;
	int height;
};

Engine* CreateRaytracerEngine(Scene *scene, int width, int height);

};

#endif
