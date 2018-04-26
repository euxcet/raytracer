#include "camera.h"

namespace Raytracer {

Camera::Camera(const Point3& eye, const Vector3& _direction,
               const Vector3& _up, const Point3& des, int width, int height)
    : eye(eye), direction(Normalize(_direction)), up(_up),
  des(des), width(width), height(height) {
    right = Normalize(Cross(direction, up));
    up = Normalize(Cross(right, direction));
    data = new Color*[width];
    for(int i = 0; i < width; i++)
        data[i] = new Color[height];
}

Ray Camera::Emit(float x, float y) {
    float dx = (x - width / 2) / height;
    float dy = y / height - 0.5;
    float dis = (des - eye).Length();
    return Ray(eye, Normalize(des + dx * dis * 2 * right + dy * dis * 2 * up - eye));
}

void Camera::SetColor(int x, int y, const Color& c) {
    data[x][y] = c;
}

void Camera::AddColor(int x, int y, const Color& c) {
    data[x][y] += c;
}

void Camera::clear() {
    for(int j = 0; j < height; j++)
        for(int i = 0; i < width; i++) data[i][j] = Color(0, 0, 0);
}

void Camera::print(int count) {
    ofstream out("1.ppm");
    out << "P3\n" << width << " " << height << "\n255\n";
    for(int j = 0; j < height; j++)
        for(int i = width - 1; i >= 0; i--) { // WARNING: !!!
            int r = fmin(data[i][j].r / count * 255., 255.);
            int g = fmin(data[i][j].g / count * 255., 255.);
            int b = fmin(data[i][j].b / count * 255., 255.);
            out << r << " " << g << " " << b << endl;
        }
}

Ray FocusCamera::Emit(float x, float y) {
    float ex = (RAND() - 0.5) / 2;
    float ey = (RAND() - 0.5) / 2;
    ex = 0;
    ey = 0;
    float dx = (x - width / 2) / height;
    float dy = y / height - 0.5;
    float dis = (des - eye).Length();
    Point3 origin = eye + ex * right + ey * up;
    return Ray(origin, Normalize(des + dx * dis * right + dy * dis * up - origin));
}

}
