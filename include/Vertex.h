#ifndef VERTEX_H
#define VERTEX_H

struct vec2 {
    float x, y;

    vec2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
};

struct vec3 {
    float x, y, z;

    vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}
};

typedef struct vec4 {
    float x, y, z, w;

    vec4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) : x(x), y(y), z(z), w(w) {}
} vec4;

struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 texcoords;

    Vertex(vec3 pos = vec3(), vec3 norm = vec3(), vec2 tex = vec2()): position(pos), normal(norm), texcoords(tex) {}
};

#endif