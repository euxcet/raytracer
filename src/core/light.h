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
    virtual Point3 GetOrigin() const = 0;
    virtual double CalcShade(const Scene *scene, const Point3 &p) = 0;
    virtual bool Intersect(const Ray &ray) const = 0;
    virtual vector<Photon*> EmitPhotons(int pcount) const = 0;

protected:
    Color color;
};

class PointLight : public Light {
public:
    PointLight(const Point3 &origin, const Color &color)
        : origin(origin), Light(color) {}
    Point3 GetOrigin() const { return origin; }
    double CalcShade(const Scene *scene, const Point3 &p);
    bool Intersect(const Ray &ray) const { return false; }
    vector<Photon*> EmitPhotons(int pcount) const;

private:
    Point3 origin;
};

class AreaLight : public Light {
public:
    AreaLight(const Point3 &origin, const Vector3 &dx, const Vector3 &dy, const Color &color)
        : origin(origin), dx(dx), dy(dy), Light(color) {}
    Point3 GetOrigin() const { return origin; }
    double CalcShade(const Scene *scene, const Point3 &p);
    bool Intersect(const Ray &ray) const;
    vector<Photon*> EmitPhotons(int pcount) const;

private:
    Point3 origin;
    Vector3 dx;
    Vector3 dy;
};

}

#endif
