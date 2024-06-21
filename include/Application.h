#ifndef APPLICATION_H
#define APPLICATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "../common/GLShader.h"

#include "Vertex.h"
#include "Mesh.h"
#include "Mat4.h"
#include "Texture.h"

class Application
{
public:
    void Initialize(GLFWwindow *window, int width, int height, const std::string &object_filename = "");
    void Render();
    void Terminate();
    void ResizeWindow(int width, int height);

private:
    GLFWwindow *m_window;
    int m_windowWidth, m_windowHeight;

    GLShader m_basicProgram;
    std::vector<Mesh> m_meshes;
    Mat4 m_worldMatrix, m_viewMatrix, m_projectionMatrix;
    
    Texture* m_textures;
};

#endif