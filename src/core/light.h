#ifndef LIGHT_H
#define LIGHT_H

#include "raytracer.h"
#include "intersection.h"
#include "scene.h"
#include "geometry.h"

namespace Raytracer {

class Light {
public:
    Light(const Color &color) : color(color) {}
    Color GetColor() { return color; }
    virtual double CalcShade(const Scene *scene, const Point3 &p) = 0;
    virtual bool Intersect(const Ray &ray) const = 0;
    virtual Point3 GetOrigin() const = 0;

protected:
    Color color;
};

class PointLight : public Light {
public:
    PointLight(const Point3 &origin, const Color &color)
        : origin(origin), Light(color) {}
    Point3 GetOrigin() const { return origin; }
    bool Intersect(const Ray &ray) const {
        return false;
    }
    double CalcShade(const Scene *scene, const Point3 &p);

private:
    Point3 origin;
};

}

#endif
