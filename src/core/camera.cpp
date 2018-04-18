#include "camera.h"

namespace Raytracer {

Camera::Camera(const Point3& eye, const Vector3& _direction,
               const Vector3& _up, int width, int height)
    : eye(eye), direction(Normalize(_direction)), up(_up),
  width(width), height(height) {
    right = Normalize(Cross(direction, up));
    up = Normalize(Cross(right, direction));
    data = new Color*[width];
    for(int i = 0; i < width; i++)
        data[i] = new Color[height];
}

Ray Camera::Emit(float x, float y) {
    x += RAND() * 1 - 0.5;
    y += RAND() * 1 - 0.5;
    float dx = (x - width / 2) / height;
    float dy = y / height - 0.5;
    Point3 onimage = eye + direction * 2 + dx * right + dy * up;
    return Ray(eye, Normalize(onimage - eye));
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
        for(int i = 0; i < width; i++) {
            int r = fmin(data[i][j].r / count * 255., 255.);
            int g = fmin(data[i][j].g / count * 255., 255.);
            int b = fmin(data[i][j].b / count * 255., 255.);
            out << r << " " << g << " " << b << endl;
        }
}

Ray FocusCamera::Emit(float x, float y) {
    float ex = (RAND() - 0.5) / 3;
    float ey = (RAND() - 0.5) / 3;
    float dx = (x - width / 2) / height;
    float dy = y / height - 0.5;
    float dis = (des - eye).Length();
    Point3 origin = eye + ex * right + ey * up;
//    cout << origin << " " << des + dx * dis * right + dy * dis * up  << endl;
    return Ray(origin, Normalize(des + dx * dis * right + dy * dis * up - origin));
}

}
