#include "Mat3.h"

Mat3::Mat3() { loadIdentity(); }

void Mat3::loadIdentity() {
    memset(data, 0, sizeof(data));
    data[0] = data[4] = data[8] = 1.0f;
}

Mat3 Mat3::inverse() const {
    Mat3 result;
    // Calcul de l'inverse
    float det = data[0] * (data[4] * data[8] - data[7] * data[5]) -
                data[3] * (data[1] * data[8] - data[7] * data[2]) +
                data[6] * (data[1] * data[5] - data[4] * data[2]);
    // Retourne la matrice originale si non inversible
    if (det == 0) return *this;
    
    float invdet = 1 / det;

    result.data[0] =  (data[4] * data[8] - data[7] * data[5]) * invdet;
    result.data[1] = -(data[1] * data[8] - data[7] * data[2]) * invdet;
    result.data[2] =  (data[1] * data[5] - data[4] * data[2]) * invdet;
    result.data[3] = -(data[3] * data[8] - data[6] * data[5]) * invdet;
    result.data[4] =  (data[0] * data[8] - data[6] * data[2]) * invdet;
    result.data[5] = -(data[0] * data[5] - data[3] * data[2]) * invdet;
    result.data[6] =  (data[3] * data[7] - data[6] * data[4]) * invdet;
    result.data[7] = -(data[0] * data[7] - data[6] * data[1]) * invdet;
    result.data[8] =  (data[0] * data[4] - data[3] * data[1]) * invdet;

    return result;
}

/**
 * OpenGL est column-major
 *    x      y     z  
 * | m[0]  m[3]  m[6] |
 * | m[1]  m[4]  m[7] |
 * | m[2]  m[5]  m[8] |
 */
Mat3 Mat3::transpose() const {
    Mat3 result;

    result.data[0] = data[0];
    result.data[1] = data[3];
    result.data[2] = data[6];
    
    result.data[3] = data[1];
    result.data[4] = data[4];
    result.data[5] = data[7];

    result.data[6] = data[2];
    result.data[7] = data[5];
    result.data[8] = data[8];

    return result;
}

Mat3 Mat3::convertToMat3(const float* mat4Data) {
    Mat3 result = Mat3();
    result.data[0] = mat4Data[0];
    result.data[1] = mat4Data[1];
    result.data[2] = mat4Data[2];
    result.data[3] = mat4Data[4];
    result.data[4] = mat4Data[5];
    result.data[5] = mat4Data[6];
    result.data[6] = mat4Data[8];
    result.data[7] = mat4Data[9];
    result.data[8] = mat4Data[10];
    return result;
}

