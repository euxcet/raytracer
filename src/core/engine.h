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
	Engine() {}
	virtual bool Render() = 0;
};

}

#endif
