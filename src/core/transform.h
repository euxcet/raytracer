#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "raytracer.h"
#include "geometry.h"

namespace Raytracer {

struct Matrix {
    Matrix() {
        for(int i = 0; i < 4; i++)
            for(int j = 0; j < 4; j++) m[i][j] = 0;
        for(int i = 0; i < 4; i++) m[i][i] = 1;
    }
    Matrix(const float mat[4][4]) {
        for(int i = 0; i < 4; i++)
            for(int j = 0; j < 4; j++) m[i][j] = mat[i][j];
    }
    Matrix(float m00, float m01, float m02, float m03,
           float m10, float m11, float m12, float m13,
           float m20, float m21, float m22, float m23,
           float m30, float m31, float m32, float m33) {
               m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
               m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
               m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
               m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
           }
    bool operator==(const Matrix &_m) const {
        for(int i = 0; i < 4; i++)
            for(int j = 0; j < 4; j++)
                if (m[i][j] != _m.m[i][j]) return false;
        return true;
    }
    bool operator!=(const Matrix &_m) const {
        for(int i = 0; i < 4; i++)
            for(int j = 0; j < 4; j++)
                if (m[i][j] != _m.m[i][j]) return true;
        return false;
    }
    friend Matrix Transpose(const Matrix &m);
    friend Matrix Inverse(const Matrix &m);
    float m[4][4];
};

inline std::ostream& operator<<(std::ostream &os, const Matrix &m) {
    os << "[\n";
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++)
            os << m.m[i][j] << ", ";
        os << endl;
    }
    os << "]\n";
	return os;
}

inline Matrix operator *(const Matrix &m1, const Matrix &m2) {
    Matrix res;
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++) res.m[i][j] = 0;
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            for(int k = 0; k < 4; k++)
                res.m[i][j] += m1.m[i][k] * m2.m[k][j];
    return res;
}


class Transform {
public:
    Transform() {}
    Transform(const Matrix &mat) {
        m = mat;
        mInv = Inverse(m);
    }
    Transform(const Matrix &mat, const Matrix &matInv) {
        m = mat;
        mInv = matInv;
    }
    Transform operator *(const Transform &_m) const {
        return Transform(m * _m.m, _m.mInv * mInv);
    }
    Transform& operator *=(const Transform &_m) {
        m = m * _m.m; mInv = _m.mInv * mInv;
        return *this;
    }

    Point3 operator()(const Point3 &p) const;
    Vector3 operator()(const Vector3 &v) const;
    Normal3 operator()(const Normal3 &n) const;

    Matrix m;
    Matrix mInv;
};

inline std::ostream& operator<<(std::ostream &os, const Transform &t) {
    os << t.m;
	return os;
}

Transform Translate(const Vector3 &delta);
Transform Scale(const Vector3 &scale);
Transform RotateX(float theta);
Transform RotateY(float theta);
Transform RotateZ(float theta);

}

#endif
