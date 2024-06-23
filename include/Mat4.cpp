#include "Mat4.h"

using namespace std;

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
    float f = 1.0f / tan(fov * 0.5f);

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
    float c = cos(rad);
    float s = sin(rad);
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

Mat3 Mat4::getNormalMatrix() {
    Mat3 worldMat3x3 = Mat3::convertToMat3(data);
    return worldMat3x3.inverse().transpose();
}

/**
 * @param position Position de la camera
 * @param target position de la cible de la camera
 * @param up Vecteur indiquant la direction du haut
 */
void Mat4::lookAt(const vec3& position, const vec3& target, const vec3& up)
{
    /**
     * Vecteur “forward” de la nouvelle orientation.
     * -(target-position) normalisé = vec3::normalize(position - target)
     */
    vec3 forward = vec3::normalize(position - target);

    /**
     * Vecteur “right”. Il est formé par le produit vectoriel entre le vecteur “forward” et “up”.
     * Le produit vectoriel suit toujours la règle de la main-droite, à savoir, intuitivement, alignez le
     * premier vecteur sur l’index de votre main droite, le second vecteur sur votre majeur, la direction du
     * résultat est indiquée par votre pouce.
     * Ici, il faut donc que le premier vecteur soit “up” et le second “forward”.
     */
    vec3 right = vec3::normalize(vec3::cross(up, forward));

    /**
     * Vecteur “up” corrigé. Il est formé par le produit vectoriel entre le vecteur “forward” et “right”
     * Aligne le premier vecteur sur le pouce de votre main droite, le second vecteur sur votre index, 
     * la direction du résultat est indiquée par votre majeur. Donc que le premier vecteur soit “forward” et le second “right”
     */
    vec3 up_corrected = vec3::cross(forward, right);

    // View Matrix
    loadIdentity();

    data[0] = right.x; data[1] = up_corrected.x; data[2]  = forward.x;
    data[4] = right.y; data[5] = up_corrected.y; data[6]  = forward.y;
    data[8] = right.z; data[9] = up_corrected.z; data[10] = forward.z;

    // Translation
    data[12] = -vec3::dot(right, position);
    data[13] = -vec3::dot(up_corrected, position);
    data[14] = -vec3::dot(forward, position);
    data[15] = 1.0f;
}