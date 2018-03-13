#ifndef RAY_H
#define RAY_H


namespace Raytracer {


class Ray {
public:
	Ray() : origin(vec3(0, 0, 0)), direction(vec3(0, 0, 0)) {DEBUG = 0;}
	Ray(const vec3& _origin, const vec3& _direction) :
		origin(_origin), direction(_direction) {}
	void SetOrigin(const vec3& _origin) { origin = _origin; }
	void setDirection(const vec3& _direction) { direction = _direction; }
	vec3 GetOrigin() const { return origin; }
	vec3 GetDirection() const { return direction; }

public:
	int DEBUG;

private:
	vec3 origin;
	vec3 direction;
};

};


#endif
