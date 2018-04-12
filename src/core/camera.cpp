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
    Vector3 Camera::Emit(double x, double y) {
        x += (rand() % 1000 / 1000.) * 1.2 - 0.6;
        y += (rand() % 1000 / 1000.) * 1.2 - 0.6;
        float dx = (x - width / 2) / height;
        float dy = y / height - 0.5;
        Point3 onimage = eye + direction + dx * right + dy * up;
        return Normalize(onimage - eye);
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

    void Camera::print() {
        ofstream out("1.ppm");
        out << "P3\n" << width << " " << height << "\n255\n";
        for(int j = 0; j < height; j++)
            for(int i = 0; i < width; i++) {
                int r = fmin(data[i][j].r * 255, 255.);
                int g = fmin(data[i][j].g * 255, 255.);
                int b = fmin(data[i][j].b * 255, 255.);
                out << r << " " << g << " " << b << endl;
            }
    }


}
