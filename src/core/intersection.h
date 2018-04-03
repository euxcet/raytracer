#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "raytracer.h"
#include "geometry.h"
#include "primitive.h"
#include "light.h"

namespace Raytracer {

class Intersection {
public:
    Intersection() { primitive = NULL; light = NULL; }

    Point3 p;
    Normal3 n;
    float dist;
    int hit; // outside: 1   inside: -1   miss: 0
    const Primitive *primitive;
    Light *light;
};



}

#endif
