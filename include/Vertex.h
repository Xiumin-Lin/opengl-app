#ifndef VERTEX_H
#define VERTEX_H

#define _USE_MATH_DEFINES
#include <cmath>
using namespace std;

struct vec2
{
    float x, y;

    explicit vec2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
};

struct vec3
{
    float x, y, z;

    explicit vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}

    vec3 operator*(float scalar) const
    {
        return vec3(x * scalar, y * scalar, z * scalar);
    }

    vec3 operator+(const vec3 &other) const
    {
        return vec3(x + other.x, y + other.y, z + other.z);
    }

    vec3 operator-(const vec3 &other) const
    {
        return vec3(x - other.x, y - other.y, z - other.z);
    }

    void operator+= (const vec3 &other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
    }

    void operator-= (const vec3 &other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
    }

    static vec3 normalize(const vec3 &v)
    {
        float norm = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
        if (norm == 0) return vec3(0, 0, 0);  // Retourner un vecteur nul si la norme est zéro
        return vec3(v.x / norm, v.y / norm, v.z / norm);
    }

    // produit vectoriel
    static vec3 cross(const vec3 &v1, const vec3 &v2)
    {
        return vec3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
    }

    // produit scalaire
    static float dot(const vec3 &v1, const vec3 &v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }
};

struct vec4
{
    float x, y, z, w;

    explicit vec4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) : x(x), y(y), z(z), w(w) {}
};

struct Vertex
{
    vec3 position;
    vec3 normal;
    vec2 texcoords;

    explicit Vertex(vec3 pos = vec3(), vec3 norm = vec3(), vec2 tex = vec2()) : position(pos), normal(norm), texcoords(tex) {}
};

#endif