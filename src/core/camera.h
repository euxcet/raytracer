#ifndef CAMERA_H
#define CAMERA_H

#include "raytracer.h"
#include "geometry.h"

namespace Raytracer {

class Camera {
public:
    Camera(const Point3& eye, const Vector3& direction,
           const Vector3& _up, int width, int height);
    Vector3 Emit(double x, double y);
    void SetColor(int x, int y, const Color& c);
    void print();

private:
    Point3 eye;
    Vector3 direction;
    Vector3 up;
    Vector3 right;
    Color **data;
	ofstream out;
    int width;
    int height;
};

}

#endif
