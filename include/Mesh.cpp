#include "Mesh.h"
#include <iostream>

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

void Mesh::GenerateGLBuffers() {
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);
    // glGenVertexArrays(1, &VAO);
}

void Mesh::DeleteGLBuffers() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
    // glDeleteVertexArrays(1, &VAO);
}

void Mesh::SetLocation(int positionLocation, int normalLocation, int texcoordLocation) {
    this->positionLocation = positionLocation;
    this->normalLocation = normalLocation;
    this->texcoordLocation = texcoordLocation;
}

/**
 * @brief Bind the VBO and IBO to the current OpenGL context
 * And setup the vertex attributes
 * DO NOT FORGET TO UNBIND THE VBO AND IBO AFTER CALLING THIS FUNCTION
 */
void Mesh::ConfigRenderParameters() {        
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(positionLocation);
        glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));

        if (normalLocation >= 0) {
            glEnableVertexAttribArray(normalLocation);
            glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
        }

        if (texcoordLocation >= 0) {
            glEnableVertexAttribArray(texcoordLocation);
            glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texcoords));
        }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

}