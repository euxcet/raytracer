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
	Engine(Scene* scene, Camera* camera, int width, int height) :
		scene(scene), camera(camera), width(width), height(height) {}
	virtual bool Render() = 0;

protected:
	Scene* scene;
	int width;
	int height;
    Camera *camera;
};

}

#endif
