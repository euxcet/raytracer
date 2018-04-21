#ifndef ENGINE_RT_H
#define ENGINE_RT_H

#include "raytracer.h"
#include "engine.h"

namespace Raytracer {

class RaytracerEngine : public Engine {
public:
    RaytracerEngine(Scene* scene, Camera *camera, int width, int height)
        : Engine(scene, camera, width, height) {}

	Color Raytrace(const Ray& ray, int depth, float index);
	Color SpawnReflectionRay(const Ray &ray, Intersection isc, int depth, float index);
	Color SpawnRefractionRay(const Ray &ray, Intersection isc, int depth, float index);
	bool Render();
};

Engine* CreateRaytracerEngine(Scene *scene, Camera *camera, int width, int height);

};

#endif
