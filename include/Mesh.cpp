#include "Mesh.h"

Mesh Mesh::GenereTriangle() {
    Mesh triangleMesh;
    triangleMesh.vertexCount = 3;

    Vertex v1 = {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}};
    Vertex v2 = {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}};
    Vertex v3 = {{0.0f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}};

    triangleMesh.vertices.push_back(v1);
    triangleMesh.vertices.push_back(v2);
    triangleMesh.vertices.push_back(v3);

    triangleMesh.indices.push_back(0);
    triangleMesh.indices.push_back(1);
    triangleMesh.indices.push_back(2);

    return triangleMesh;
}