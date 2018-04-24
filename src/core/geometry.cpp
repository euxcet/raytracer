#include "geometry.h"

namespace Raytracer {

Vector3 Matrix3::operator*(const Vector3 &v) const {
    float x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z;
    float y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z;
    float z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z;
    return Vector3(x, y, z);
}

Matrix3 Transpose(const Matrix3 &m) {
    Matrix3 mat;
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++) mat.m[i][j] = m.m[j][i];
    return mat;
}

Matrix3 Inverse(const Matrix3 &m) {
    int indxc[3], indxr[3];
    int ipiv[3] = {0, 0, 0};
    float minv[3][3];
    memcpy(minv, m.m, 3 * 3 * sizeof(float));
    for (int i = 0; i < 3; i++) {
        int irow = 0, icol = 0;
        float big = 0.f;
        for (int j = 0; j < 3; j++) {
            if (ipiv[j] != 1) {
                for (int k = 0; k < 3; k++) {
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
            for (int k = 0; k < 3; ++k) std::swap(minv[irow][k], minv[icol][k]);
        }
        indxr[i] = irow;
        indxc[i] = icol;
        if (minv[icol][icol] == 0.f) puts("Error: Singular matrix in MatrixInvert");
        float pivinv = 1. / minv[icol][icol];
        minv[icol][icol] = 1.;
        for (int j = 0; j < 3; j++) minv[icol][j] *= pivinv;
        for (int j = 0; j < 3; j++) {
            if (j != icol) {
                float save = minv[j][icol];
                minv[j][icol] = 0;
                for (int k = 0; k < 3; k++) minv[j][k] -= minv[icol][k] * save;
            }
        }
    }
    for (int j = 2; j >= 0; j--) {
        if (indxr[j] != indxc[j]) {
            for (int k = 0; k < 3; k++)
                std::swap(minv[k][indxr[j]], minv[k][indxc[j]]);
        }
    }
    return Matrix3(minv);
}
}
