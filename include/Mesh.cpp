#include "Mesh.h"
#include <iostream>

Mesh Mesh::GenereTriangle() {
    Mesh triangleMesh;
    triangleMesh.vertexCount = 3;

    Vertex v1 = {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}};
    Vertex v2 = {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}};
    Vertex v3 = {{0.0f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.5f, 1.0f}};

    triangleMesh.vertices = {v1, v2, v3};
    triangleMesh.indices = {0, 1, 2};

    return triangleMesh;
}

Mesh Mesh::GenererRectangle() {
    Mesh rectangleMesh;
    rectangleMesh.vertexCount = 4;

    Vertex v1 = {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}};
    Vertex v2 = {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}};
    Vertex v3 = {{0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}};
    Vertex v4 = {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}};

    rectangleMesh.vertices = {v1, v2, v3, v4};
    rectangleMesh.indices = {0, 1, 2, 0, 2, 3};

    return rectangleMesh;
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

void Mesh::SetAttribLocation(int positionLocation, int normalLocation, int texcoordLocation) {
    this->positionLocation = positionLocation;
    this->normalLocation = normalLocation;
    this->texcoordLocation = texcoordLocation;
}

/**
 * @brief Bind the VBO and IBO to the current OpenGL context
 * And setup the vertex attributes
 * DO NOT FORGET TO UNBIND THE VBO AND IBO AFTER CALLING THIS FUNCTION
 */
void Mesh::Draw() {        
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

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
        
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}