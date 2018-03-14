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
using std::ofstream;
using std::cin;
using std::cout;
using std::endl;
using std::shared_ptr;
using std::abs;

#include <vector>
using std::vector;

namespace Raytracer {

#define EPS (1e-4)
#define BIGFLOAT (1e6)
#define TRACEDEPTH (6)

#define HIT 1
#define MISS 0
#define INPRIM -1

class Vector3;
class Plane;
class aabb;
class Engine;


class Ray;
class Primitive;
class Material;
class Scene;
class PPM;


};

#endif
