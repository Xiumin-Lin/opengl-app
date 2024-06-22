#ifndef MAT4_H
#define MAT4_H

#include "Mat3.h"
#include "Vertex.h"

struct Mat4
{
    /**
     * OpenGL est column-major
     * x, y & z sont les coordonnées de translation
     * w est la coordonnée homogène
     *    x      y     z      w
     * | m[0]  m[4]  m[8]   m[12] |
     * | m[1]  m[5]  m[9]   m[13] |
     * | m[2]  m[6]  m[10]  m[14] |
     * | m[3]  m[7]  m[11]  m[15] |
     */
    float data[16];

    Mat4();

    void loadIdentity();
    static void ortho(Mat4* m, float left, float right, float bottom, float top, float znear, float zfar);
    static void perspective(Mat4* m, float fov, float aspect, float znear, float zfar);
    void translate(float x, float y, float z);
    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);
    void scale(float x, float y, float z);
    Mat3 getNormalMatrix();
    vec3 getPosition() const { return vec3(data[12], data[13], data[14]); }

    Mat4 operator*(const Mat4 &other) const
    {
        Mat4 result;
        for (int col = 0; col < 4; ++col)
        {
            for (int row = 0; row < 4; ++row)
            {
                result.data[col * 4 + row] = 0.0f;
                for (int k = 0; k < 4; ++k)
                { 
                    // Produit scalaire de la row de `this` et de la col de `other`
                    result.data[col * 4 + row] += data[k * 4 + row] * other.data[col * 4 + k];
                }
            }
        }
        return result;
    }

    void lookAt(const vec3& position, const vec3& target, const vec3& up);
};

#endif // MAT4_H