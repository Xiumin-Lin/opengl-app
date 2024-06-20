#ifndef MESH_H
#define MESH_H

#include <GL/glew.h> // For uint32_t
#include <vector>
#include "Vertex.h"

struct Mesh {
    std::vector<Vertex> vertices;
    uint32_t vertexCount;

    Mesh() : vertexCount(0) {}
};

#endif // MESH_H