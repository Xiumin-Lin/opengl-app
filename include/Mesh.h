#ifndef MESH_H
#define MESH_H

#include <GL/glew.h> // For uint32_t
#include <vector>
#include <memory>
#include "Vertex.h"
#include "Material.h"

class Mesh {
public:
    uint32_t vertexCount = 0;
    uint32_t indexCount = 0;
    Vertex* vertices = nullptr;
    uint32_t* indices = nullptr;

    // Material
    uint32_t materialCount = 0;
    Material* materials = nullptr;

    // Verterx Buffer Object (VBO) & Index Buffer Object (IBO)
    // Vertex Array Object (VAO) is not available in OpenGL ES 2.1
    GLuint VBO = 0, IBO = 0;
    // Atrribute locations
    int positionLocation = -1, normalLocation = -1, texcoordLocation = -1;
    
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
    void allocateMaterials(size_t count);

    void generateGLBuffers();
    void deleteGLBuffers();
    void setAttribLocation(int positionLocation, int normalLocation, int texcoordLocation);
    void draw();

    static Mesh GenereTriangle();
    static Mesh GenererRectangle();
};

#endif // MESH_H