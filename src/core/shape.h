#ifndef SHAPE_H
#define SHAPE_H

#include "raytracer.h"
#include "geometry.h"
#include "intersection.h"

namespace Raytracer {

class Shape {
public:
    Shape();
    virtual ~Shape();
    virtual bool Intersect(const Ray &ray, Intersection* isc) const = 0;
	virtual Normal3 GetNormal(const Point3& pos) const = 0;
};

}
#endif
