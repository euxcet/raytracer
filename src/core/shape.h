#ifndef SHAPE_H
#define SHAPE_H

#include "raytracer.h"
#include "geometry.h"
#include "intersection.h"
#include "transform.h"

namespace Raytracer {

class Shape {
public:
    Shape(Transform transform) : transform(transform) {}
    virtual ~Shape();
    virtual bool Intersect(const Ray &ray, Intersection* isc) const = 0;
	virtual Normal3 GetNormal(const Point3& pos) const = 0;
    virtual AABB GetAABB() const = 0;
    virtual pair<float, float> Coordinate(const Point3& pos) const = 0;

protected:
    Transform transform;
};

}
#endif
