#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <algorithm>
#include <omp.h>
using std::string;
using std::min;
using std::max;

#include <iostream>
#include <fstream>
#include <cassert>
#include <memory>
using std::ifstream;
using std::ofstream;
using std::cin;
using std::cout;
using std::endl;
using std::shared_ptr;
using std::unique_ptr;
using std::abs;
using std::pair;
using std::make_pair;
using std::sort;

#include <vector>
using std::vector;

#include <string>
using std::string;


namespace Raytracer {

#define EPS (1e-4)
#define INF (1e6)
#define BIGFLOAT (1e6)
#define TRACEDEPTH (9)
#define SAMPLE (25)
#define LIGHT_SAMPLE (9)
#define PI M_PI

#define HIT 1
#define MISS 0
#define INPRIM -1

#define RAND() (rand() % 32768 / 32768.)

class Vector3;
class Point3;
class Normal3;
class AABB;
class Ray;
class Photon;

class Camera;
class FocusCamera;
class Engine;
class Primitive;
class GeometricPrimitive;
class Mesh;
class Aggregate;
class Material;
class Light;
class PointLight;
class Scene;
class Shape;
class Intersection;
class Texture;
class Transform;


};

#endif
