#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "include/Application.h"

const int WIDTH = 800;
const int HEIGHT = 600;

void window_resize_callback(GLFWwindow* window, int width, int height)
{
    // std::cout << "Resized to " << width << "x" << height << std::endl;
    Application::ResizeWindow(width, height);
}

int main(void)
{
    GLFWwindow *window;
    /* Initialize the library */
    if (!glfwInit()) return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "Exemple OpenGL 2.+", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE; // need for VAO on MacOs
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Erreur d'initialisation de GLEW" << std::endl;
        return -1;
    }

    Application app;
    app.Initialize(WIDTH, HEIGHT, "./assets/cube.obj");
    glfwSetFramebufferSizeCallback(window, window_resize_callback);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        app.Render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    app.Terminate();
    glfwTerminate();
    return 0;
}