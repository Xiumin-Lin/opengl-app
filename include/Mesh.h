#ifndef MESH_H
#define MESH_H

#include <GL/glew.h> // For uint32_t
#include <vector>
#include "Vertex.h"

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    uint32_t vertexCount;
    // VAO not available in OpenGL 2.1 officialy
    GLuint VBO, IBO;
    int positionLocation, normalLocation, texcoordLocation;
    
    Mesh() : vertexCount(0) {}
    void GenerateGLBuffers();
    void SetLocation(int positionLocation, int normalLocation, int texcoordLocation);
    void ConfigRenderParameters();
    void DeleteGLBuffers();
    static Mesh GenereTriangle();
};

#endif // MESH_H