#ifndef MAT3_H
#define MAT3_H

#include <cmath>
#include <cstring>

struct Mat3
{
public:
    float data[9]; // stockage de la matrice 3x3

    Mat3();
    void loadIdentity();
    Mat3 inverse() const;
    Mat3 transpose() const;
    static Mat3 convertToMat3(const float *mat4Data);
};

#endif