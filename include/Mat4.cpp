#include "Mat4.h"
#include <cmath>
#include <cstring>

Mat4::Mat4() { loadIdentity(); }

void Mat4::loadIdentity()
{
    memset(data, 0, sizeof(data));
    data[0] = data[5] = data[10] = data[15] = 1.0f;
}

void Mat4::ortho(Mat4* m, float left, float right, float bottom, float top, float znear, float zfar) {

    m->loadIdentity();
    m->data[0] = 2.0f / (right - left);
    m->data[5] = 2.0f / (top - bottom);
    m->data[10] = -2.0f / (zfar - znear);
    m->data[12] = -(right + left) / (right - left);
    m->data[13] = -(top + bottom) / (top - bottom);
    m->data[14] = -(zfar + znear) / (zfar - znear);
    m->data[15] = 1.0f;
}

void Mat4::perspective(Mat4* m, float fov, float aspect, float znear, float zfar)
{
    float f = 1.0f / std::tan(fov * 0.5f);

    m->loadIdentity();
    m->data[0] = f / aspect;
    m->data[5] = f;
    m->data[10] = -(zfar + znear) / (zfar - znear);
    m->data[11] = -1.0f;
    m->data[14] = -(2.0f * zfar * znear) / (zfar - znear);
    m->data[15] = 0.0f;
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
    float c = std::cos(rad);
    float s = std::sin(rad);
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

void Mat4::scale(float x, float y, float z)
{
    Mat4 scale;
    scale.data[0] = x;
    scale.data[5] = y;
    scale.data[10] = z;
    scale.data[15] = 1.0f;
    *this = *this * scale;
}

// void Mat4::invert()
// {
    
// }

// void Mat4::transpose()
// {
    
// }