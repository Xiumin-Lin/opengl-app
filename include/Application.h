#ifndef APPLICATION_H
#define APPLICATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "../common/GLShader.h"

#include "Vertex.h"
#include "Mesh.h"
#include "Mat4.h"

class Application {
public:
    void Initialize(int width, int height, const std::string &object_filename = "");
    void LoadObject(const char *filename);
    void Render();
    void Terminate();

private:
    GLShader m_basicProgram;
    GLuint VBO, IBO, VAO;
    int m_width, m_height;
    std::vector<Mesh> m_meshes;
    Mat4 m_meshMatrix;
};

#endif