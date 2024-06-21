#include "Mesh.h"
#include <iostream>
#include <cstring> // For memcpy

Mesh::~Mesh()
{
    deleteMesh();
}
// Constructeur de déplacement pour std::move
Mesh::Mesh(Mesh &&other) noexcept
{
    move(other);
    other.reset();
}

// Opérateur d'affectation de déplacement pour std::move
Mesh &Mesh::operator=(Mesh &&other) noexcept
{
    if (this != &other)
    {
        deleteMesh();
        move(other);
        other.reset();
    }
    return *this;
}

void Mesh::deleteMesh() {
    delete[] vertices;
    delete[] indices;
    deleteGLBuffers();
}

void Mesh::move(const Mesh& other) {
    vertices = other.vertices;
    indices = other.indices;
    vertexCount = other.vertexCount;
    indexCount = other.indexCount;
    
    ambientColor = other.ambientColor;
    diffuseColor = other.diffuseColor;
    specularColor = other.specularColor;
    shininess = other.shininess;

    VBO = other.VBO;
    IBO = other.IBO;
    
    positionLocation = other.positionLocation;
    normalLocation = other.normalLocation;
    texcoordLocation = other.texcoordLocation;
}

void Mesh::reset() {
    vertices = nullptr;
    indices = nullptr;
    vertexCount = 0;
    indexCount = 0;
    ambientColor = vec3(1.0f, 1.0f, 1.0f);
    diffuseColor = vec3(1.0f, 1.0f, 1.0f);
    specularColor = vec3(1.0f, 1.0f, 1.0f);
    shininess = 8.0f;
    IBO = 0;
    VBO = 0;
    positionLocation = -1;
    normalLocation = -1;
    texcoordLocation = -1;
}

void Mesh::allocateVertices(size_t count)
{
    delete[] vertices;
    vertices = new Vertex[count];
    vertexCount = count;
}

void Mesh::allocateIndices(size_t count)
{
    delete[] indices;
    indices = new uint32_t[count];
    indexCount = count;
}

void Mesh::generateGLBuffers()
{
    glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(uint32_t), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::deleteGLBuffers()
{
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
    VBO = 0;
    IBO = 0;
}

void Mesh::setAttribLocation(int positionLocation, int normalLocation, int texcoordLocation)
{
    if (positionLocation < 0)
        std::cerr << "Invalid POSITION attribute location" << std::endl;
    this->positionLocation = positionLocation;
    if (normalLocation < 0)
        std::cerr << "Invalid NORMAL attribute location" << std::endl;
    this->normalLocation = normalLocation;
    if (texcoordLocation < 0)
        std::cerr << "Invalid TEX COORD attribute location" << std::endl;
    this->texcoordLocation = texcoordLocation;
}

/**
 * @brief Bind the VBO and IBO to the current OpenGL context
 * And setup the vertex attributes
 * NOT TO MYSELF: DO NOT BIND VBO AND IBO SEPARATELY FROM DRAWELEMENTS
 * OTHERWISE ONLY THE LAST VBO AND IBO OF MESHES WILL BE USED
 */
void Mesh::draw()
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glEnableVertexAttribArray(positionLocation);
        glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));

        glEnableVertexAttribArray(normalLocation);
        glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));

        glEnableVertexAttribArray(texcoordLocation);
        glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texcoords));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(positionLocation);
    glDisableVertexAttribArray(normalLocation);
    glDisableVertexAttribArray(texcoordLocation);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// ===================================================================================

Mesh Mesh::GenereTriangle()
{
    Mesh triangleMesh;

    Vertex v1(vec3(-0.5f, -0.5f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f));
    Vertex v2(vec3(0.5f, -0.5f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f));
    Vertex v3(vec3(0.0f, 0.5f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.5f, 1.0f));

    triangleMesh.vertices = new Vertex[3]{v1, v2, v3};
    triangleMesh.vertexCount = 3;
    triangleMesh.indices = new uint32_t[3]{0, 1, 2};
    triangleMesh.indexCount = 3;

    return triangleMesh;
}

Mesh Mesh::GenererRectangle()
{
    Mesh rectangleMesh;

    Vertex v1(vec3(-0.5f, -0.5f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f));
    Vertex v2(vec3(0.5f, -0.5f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f));
    Vertex v3(vec3(0.5f, 0.5f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f));
    Vertex v4(vec3(-0.5f, 0.5f, 0.0f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f));

    rectangleMesh.vertices = new Vertex[4]{v1, v2, v3, v4};
    rectangleMesh.vertexCount = 4;
    rectangleMesh.indices = new uint32_t[6]{0, 1, 2, 0, 2, 3};
    rectangleMesh.indexCount = 6;

    return rectangleMesh;
}