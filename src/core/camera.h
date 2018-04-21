#ifndef CAMERA_H
#define CAMERA_H

#include "raytracer.h"
#include "geometry.h"

namespace Raytracer {

class Camera {
public:
    Camera(const Point3& eye, const Vector3& direction,
           const Vector3& _up, const Point3& des, int width, int height);
    virtual Ray Emit(float x, float y);
    void SetColor(int x, int y, const Color& c);
    void AddColor(int x, int y, const Color& c);
    void print(int count = 1);
    void clear();

protected:
    Point3 eye;
    Point3 des;
    Vector3 direction;
    Vector3 up;
    Vector3 right;
    Color **data;
	ofstream out;
    int width;
    int height;
};

class FocusCamera : public Camera {
public:
    FocusCamera(const Point3& eye, const Vector3& direction,
           const Vector3& _up, const Point3& des, int width, int height)
           : Camera(eye, direction, _up, des, width, height) {}
    Ray Emit(float x, float y);
};

}

#endif
