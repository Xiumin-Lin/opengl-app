#ifndef APPLICATION_H
#define APPLICATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include <vector>

#include "../common/GLShader.h"

#include "Vertex.h"
#include "Mat4.h"
#include "Mesh.h"
#include "Texture.h"
#include "Utils.h"


class Application
{
public:
    void Initialize(GLFWwindow *window, int width, int height, const std::string &object_filename = "", const std::string &mtl_basepath = "./");
    void Render();
    void Terminate();
    void ResizeWindow(int width, int height);

private:
    GLFWwindow *m_window;
    int m_windowWidth, m_windowHeight;

    GLShader m_basicProgram;
    std::vector<std::unique_ptr<Mesh>> m_meshes;
    Mat4 m_viewMatrix, m_projectionMatrix;
};

#endif