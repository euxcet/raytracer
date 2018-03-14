#ifndef SHAPE_H
#define SHAPE_H

#include "raytracer.h"
#include "common.h"

namespace Raytracer {

class Shape {
public:
    Shape();
    virtual ~Shape();
    virtual int Intersect(const Ray &ray, float &dist) const = 0;
	Normal3 GetNormal(const Point3& pos) const = 0;
};

}
#endif
