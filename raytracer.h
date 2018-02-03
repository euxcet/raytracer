#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "common.h"

namespace Raytracer {

class Ray {
public:
	Ray() : origin(vec3(0, 0, 0)), direction(vec3(0, 0, 0)) {}
	Ray(vec3& _origin, vec3& _direction) :
		origin(_origin), direction(_direction) {}
	void SetOrigin(vec3& _origin) { origin = _origin; }
	void setDirection(vec3& _direction) { direction = _direction; }
	vec3& GetOrigin() { return origin; }
	vec3& GetDirection() { return direction; }

private:
	vec3 origin;
	vec3 direction;
};

class Scene;
class Primitive;
class Engine {
public:
	Engine() { scene = new Scene(); }
	~Engine() { delete scene; }
	Scene* GetScene() { return scene; }
	void Init();
	bool Render();


protected:
	Scene* scene;

};

};

#endif
