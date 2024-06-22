#ifndef APPLICATION_H
#define APPLICATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include <vector>

#include "../common/GLShader.h"

#include "CameraOrbitale.h"
#include "Mat4.h"
#include "Mesh.h"
#include "Texture.h"
#include "Utils.h"
#include "Vertex.h"


class Application
{
public:
    void Initialize(GLFWwindow *window, int width, int height, float cameraInitRadium, const std::string &object_filename = "", const std::string &mtl_basepath = "./");
    void Render();
    void Terminate();
    void ResizeWindow(int width, int height);
    CameraOrbitale& GetCamera() { return m_camera; }
    void NextMeshFocus();

private:
    GLFWwindow *m_window;
    int m_windowWidth, m_windowHeight;
    int currentMeshFocusIndex = 0;

    GLShader m_basicProgram;
    std::vector<std::unique_ptr<Mesh>> m_meshes;
    Mat4 m_projectionMatrix;
    CameraOrbitale m_camera;
};

#endif