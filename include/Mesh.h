#ifndef MESH_H
#define MESH_H

#include <GL/glew.h> // For uint32_t
#include <cstring> // For memcpy
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Mat4.h"
#include "Material.h"
#include "Vertex.h"

class Mesh {
public:
    std::string name = "";
    // Mesh data
    uint32_t vertexCount = 0;
    uint32_t indexCount = 0;
    Vertex* vertices = nullptr;
    uint32_t* indices = nullptr;

    // Material
    Material* material = nullptr;

    // Reference World matrix
    Mat4 worldMatrix = Mat4();

    // Verterx Buffer Object (VBO) & Index Buffer Object (IBO)
    // Vertex Array Object (VAO) is not available in OpenGL ES 2.1
    GLuint VBO = 0, IBO = 0;
    // Atrribute locations
    int positionLocation = -1, normalLocation = -1, texcoordLocation = -1;
    // Uniform locations
    int worldMatrixLocation = -1, normalMatrixLocation = -1;
    
    Mesh() = default;
    Mesh(const Mesh& other) = delete;               // Suppression du constructeur de copie
    Mesh& operator=(const Mesh& other) = delete;    // Suppression de l'opérateur d'affectation
    Mesh(Mesh&& other) noexcept;                    // Constructeur de déplacement
    Mesh& operator=(Mesh&& other) noexcept;         // Opérateur d'affectation de déplacement
    ~Mesh();

    void move(const Mesh& other);
    void reset();
    void deleteMesh();
    void allocateVertices(size_t count);
    void allocateIndices(size_t count);
    void createMaterial();
    void setWorldMatrix(const Mat4& worldMatrix);
    vec3 getPosition() const { return worldMatrix.getPosition(); }

    void generateGLBuffers();
    void deleteGLBuffers();
    void setAttribLocation(int positionLocation, int normalLocation, int texcoordLocation);
    void setUniformLocation(int worldMatrixLocation, int normalMatrixLocation);
    void draw();
};

#endif // MESH_H