#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "raytracer.h"

namespace Raytracer {
struct Matrix {
    Matrix() {}
    Matrix(float mat[4][4]) {
        for(int i = 0; i < 4; i++)
            for(int j = 0; j < 4; j++) m[i][j] = mat[i][j];
    }
    bool operator==(const Matrix &_m) const {
        for(int i = 0; i < 4; i++)
            for(int j = 0; j < 4; j++)
                if (m[i][j] != _m[i][j]) return false;
        return true;
    }
    bool operator!=(const Matrix &_m) const {
        for(int i = 0; i < 4; i++)
            for(int j = 0; j < 4; j++)
                if (m[i][j] != _m[i][j]) return true;
        return false;
    }

    float m[4][4];
};

class Transform {
public:
    Transform() {}
    Transform(const float mat[4][4]) {
        m = Matrix(mat);
        mInv = Inverse(m);
    }
};

Transform Translate(const Vector3 &delta) {
    Matrix m(1, 0, 0, delta.x,
             0, 1, 0, delta.y,
             0, 0, 1, delta.z,
             0, 0, 0, 1);
    Matrix mInv(1, 0, 0, -delta.x,
                0, 1, 0, -delta.y,
                0, 0, 1, -delta.z);
    return Transform(m, mInv);
}

Transform Scale(const Vector3 &scale) {
    Matrix m(scale.x, 0, 0, 0,
             0, scale.y, 0, 0,
             0, 0, scale.z, 0,
             0, 0, 0, 1);
    Matrix mInv(1 / scale.x, 0, 0, 0,
                0, 1 / scale.y, 0, 0,
                0, 0, 1 / scale.z, 0,
                0, 0, 0, 1);
    return Transform(m, mInv);
}

Transform RotateX(float theta) {
    float Sin = std::sin(theta / 180. * PI);
    float Cos = std::cos(theta / 180. * PI);
    Matrix m(1, 0, 0, 0,
             0, Cos, -Sin, 0,
             0, Sin, Cos, 0,
             0, 0, 0, 1);
    Matrix mInv(1, 0, 0, 0,
                0, Cos, Sin, 0,
                0, -Sin, Cos, 0,
                0, 0, 0, 1);
    return Transform(m, mInv);
}

Transform RotateY(float theta) {
    float Sin = std::sin(theta / 180. * PI);
    float Cos = std::cos(theta / 180. * PI);
    Matrix m(Cos, 0, Sin, 0,
             0, 1, 0, 0,
             -Sin, 0, Cos, 0,
             0, 0, 0, 1);
    Matrix mInv(Cos, 0, -Sin, 0,
                0, 1, 0, 0,
                Sin, 0, Cos, 0,
                0, 0, 0, 1);
    return Transform(m, mInv);
}

Transform RotateZ(float theta) {
    float Sin = std::sin(theta / 180. * PI);
    float Cos = std::cos(theta / 180. * PI);
    Matrix m(Cos, -Sin, 0, 0
             Sin, Cos, 0, 0
             0, 0, 1, 0
             0, 0, 0, 1);
    Matrix mInv(Cos, Sin, 0, 0
                -Sin, Cos, 0, 0
                0, 0, 1, 0
                0, 0, 0, 1);
    return Transform(m, mInv);
}


}

#endif
