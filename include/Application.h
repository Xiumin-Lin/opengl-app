#ifndef APPLICATION_H
#define APPLICATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
// #include <memory>
#include <vector>

#include "../common/GLShader.h"

#include "CameraOrbitale.h"
#include "Mat4.h"
#include "Mesh.h"
#include "Object.h"
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
    void NextObjectFocus();

private:
    GLFWwindow *m_window;
    int m_windowWidth, m_windowHeight;
    int currentObjectFocusIndex = 0;

    GLShader m_basicProgram;
    std::vector<Object> m_objects;
    Mat4 m_projectionMatrix;
    CameraOrbitale m_camera;
};

#endif