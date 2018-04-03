#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
using std::string;

#include <iostream>
#include <fstream>
#include <cassert>
#include <memory>
using std::ofstream;
using std::cin;
using std::cout;
using std::endl;
using std::shared_ptr;
using std::unique_ptr;
using std::abs;

#include <vector>
using std::vector;

namespace Raytracer {

#define EPS (1e-4)
#define INF (1e6)
#define BIGFLOAT (1e6)
#define TRACEDEPTH (6)

#define HIT 1
#define MISS 0
#define INPRIM -1

class Vector3;
class Point3;
class Normal3;
class aabb;
class Ray;
class Engine;

class Primitive;
class GeometricPrimitive;
class Aggregate;
class Material;
class Light;
class Scene;
class Shape;
class Intersection;


};

#endif
