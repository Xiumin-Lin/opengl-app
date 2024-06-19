#include <cmath>
#include "Mat4.h"

Mat4::Mat4() { loadIdentity(); }

void Mat4::loadIdentity()
{
    for (int i = 0; i < 16; i++)
    {
        data[i] = 0.0f;
    }
    data[0] = data[5] = data[10] = data[15] = 1.0f;
}

void Mat4::perspective(float fov, float aspect, float znear, float zfar)
{
    float f = 1.0f / std::tan(fov * 0.5f);
    loadIdentity();
    data[0] = f / aspect;
    data[5] = f;
    data[10] = (zfar + znear) / (znear - zfar);
    data[11] = -1.0f;
    data[14] = (2.0f * zfar * znear) / (znear - zfar);
    data[15] = 0.0f;
}

void Mat4::translate(float x, float y, float z)
{
    loadIdentity();
    data[12] = x;
    data[13] = y;
    data[14] = z;
}

void Mat4::rotateZ(float angle)
{
    float rad = angle * M_PI / 180.0f;
    float c = std::cos(rad);
    float s = std::sin(rad);
    loadIdentity();
    data[0] = c;
    data[4] = -s;
    data[1] = s;
    data[5] = c;
}
