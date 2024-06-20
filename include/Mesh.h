#ifndef MESH_H
#define MESH_H

#include <GL/glew.h> // For uint32_t
#include <vector>
#include "Vertex.h"

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    uint32_t vertexCount;

    Mesh() : vertexCount(0) {}
};

#endif // MESH_H