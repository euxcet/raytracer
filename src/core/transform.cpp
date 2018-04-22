#include "transform.h"

namespace Raytracer {

Matrix Transpose(const Matrix &m) {
    Matrix mat;
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++) mat.m[i][j] = m.m[i][j];
    return mat;
}

Matrix Inverse(const Matrix &m) {
    int indxc[4], indxr[4];
    int ipiv[4] = {0, 0, 0, 0};
    float minv[4][4];
    memcpy(minv, m.m, 4 * 4 * sizeof(float));
    for (int i = 0; i < 4; i++) {
        int irow = 0, icol = 0;
        float big = 0.f;
        for (int j = 0; j < 4; j++) {
            if (ipiv[j] != 1) {
                for (int k = 0; k < 4; k++) {
                    if (ipiv[k] == 0) {
                        if (std::abs(minv[j][k]) >= big) {
                            big = float(std::abs(minv[j][k]));
                            irow = j;
                            icol = k;
                        }
                    } else if (ipiv[k] > 1)
                        puts("Error: Singular matrix in MatrixInvert");
                }
            }
        }
        ++ipiv[icol];
        if (irow != icol) {
            for (int k = 0; k < 4; ++k) std::swap(minv[irow][k], minv[icol][k]);
        }
        indxr[i] = irow;
        indxc[i] = icol;
        if (minv[icol][icol] == 0.f) puts("Error: Singular matrix in MatrixInvert");

        float pivinv = 1. / minv[icol][icol];
        minv[icol][icol] = 1.;
        for (int j = 0; j < 4; j++) minv[icol][j] *= pivinv;

        for (int j = 0; j < 4; j++) {
            if (j != icol) {
                float save = minv[j][icol];
                minv[j][icol] = 0;
                for (int k = 0; k < 4; k++) minv[j][k] -= minv[icol][k] * save;
            }
        }
    }
    for (int j = 3; j >= 0; j--) {
        if (indxr[j] != indxc[j]) {
            for (int k = 0; k < 4; k++)
                std::swap(minv[k][indxr[j]], minv[k][indxc[j]]);
        }
    }
    return Matrix(minv);
}

Transform Translate(const Vector3 &delta) {
    Matrix m(1, 0, 0, delta.x,
             0, 1, 0, delta.y,
             0, 0, 1, delta.z,
             0, 0, 0, 1);
    Matrix mInv(1, 0, 0, -delta.x,
                0, 1, 0, -delta.y,
                0, 0, 1, -delta.z,
                0, 0, 0, 1);
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
    Matrix m(Cos, -Sin, 0, 0,
             Sin, Cos, 0, 0,
             0, 0, 1, 0,
             0, 0, 0, 1);
    Matrix mInv(Cos, Sin, 0, 0,
                -Sin, Cos, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1);
    return Transform(m, mInv);
}

Point3 Transform::operator()(const Point3 &p) const {
    float x = m.m[0][0] * p.x + m.m[0][1] * p.y + m.m[0][2] * p.z + m.m[0][3];
    float y = m.m[1][0] * p.x + m.m[1][1] * p.y + m.m[1][2] * p.z + m.m[1][3];
    float z = m.m[2][0] * p.x + m.m[2][1] * p.y + m.m[2][2] * p.z + m.m[2][3];
    float w = m.m[3][0] * p.x + m.m[3][1] * p.y + m.m[3][2] * p.z + m.m[3][3];
    return Point3(x, y, z) / w;
}

Vector3 Transform::operator()(const Vector3 &v) const {
    float x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z;
    float y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z;
    float z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z;
    return Vector3(x, y, z);
}

Normal3 Transform::operator()(const Normal3 &n) const {
    float x = mInv.m[0][0] * n.x + mInv.m[0][1] * n.y + mInv.m[0][2] * n.z;
    float y = mInv.m[1][0] * n.x + mInv.m[1][1] * n.y + mInv.m[1][2] * n.z;
    float z = mInv.m[2][0] * n.x + mInv.m[2][1] * n.y + mInv.m[2][2] * n.z;
    return Normal3(x, y, z);
}

}
