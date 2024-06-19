#ifndef VERTEX_H
#define VERTEX_H

struct vec2 {
    float x, y;
};

struct vec3 {
    float x, y, z;
};

struct vec4 {
    float x, y, z, w;
};

struct Vertex {
    vec2 position;
    vec3 color;
};

#endif