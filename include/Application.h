#ifndef APPLICATION_H
#define APPLICATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "../common/GLShader.h"

#include "Vertex.h"

class Application {
public:
    void Initialize(int width, int height);
    void Render();
    void Terminate();

private:
    GLShader m_basicProgram;
    GLuint VBO, IBO, VAO;
    int m_width, m_height;
};

#endif