#ifndef LIGHT_H
#define LIGHT_H

#include "raytracer.h"
#include "geometry.h"

namespace Raytracer {

class Light {
public:
    Light(const Point3 &origin, const Color &color)
        : origin(origin), color(color) {}

    Color GetColor() { return color; }
    Point3 GetOrigin() const { return origin; }
    int Intersect(const Ray &ray) {
        return 0;
    }

protected:
    Point3 origin;
    Color color;
};

}

#endif
