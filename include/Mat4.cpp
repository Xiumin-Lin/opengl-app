#include "Mat4.h"
#include <cmath>
#include <cstring>

Mat4::Mat4() { loadIdentity(); }

void Mat4::loadIdentity()
{
    memset(data, 0, sizeof(data));
    data[0] = data[5] = data[10] = data[15] = 1.0f;
}

Mat4 Mat4::perspective(float fov, float aspect, float znear, float zfar)
{
    float f = 1.0f / tan(fov * 0.5f);
    Mat4 result;

    result.data[0] = f / aspect;
    result.data[5] = f;
    result.data[10] = (zfar + znear) / (znear - zfar);
    result.data[11] = -1.0f;
    result.data[14] = (2.0f * zfar * znear) / (znear - zfar);
    result.data[15] = 0.0f;
    return result;
}

void Mat4::translate(float x, float y, float z)
{
    Mat4 translation;
    translation.data[12] = x;
    translation.data[13] = y;
    translation.data[14] = z;
    *this = *this * translation;
}

void Mat4::rotateX(float angle)
{
    float rad = angle * M_PI / 180.0f;
    float c = cos(rad);
    float s = sin(rad);
    Mat4 rotation;
    rotation.data[5] = c;
    rotation.data[6] = s;
    rotation.data[9] = -s;
    rotation.data[10] = c;
    *this = *this * rotation;
}

void Mat4::rotateY(float angle)
{
    float rad = angle * M_PI / 180.0f;
    float c = cos(rad);
    float s = sin(rad);
    Mat4 rotation;
    rotation.data[0] = c;
    rotation.data[2] = -s;
    rotation.data[8] = s;
    rotation.data[10] = c;
    *this = *this * rotation;
}

void Mat4::rotateZ(float angle)
{
    float rad = angle * M_PI / 180.0f;
    float c = std::cos(rad);
    float s = std::sin(rad);
    Mat4 rotation;
    rotation.data[0] = c;
    rotation.data[1] = s;
    rotation.data[4] = -s;
    rotation.data[5] = c;
    *this = *this * rotation;
}
