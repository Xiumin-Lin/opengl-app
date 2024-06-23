#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "include/Application.h"

const int WIDTH = 800;
const int HEIGHT = 800;

int main(int argc, char** argv)
{
    GLFWwindow *window;
    /* Initialize the library */
    if (!glfwInit()) return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "APP OpenGL 2.1", NULL, NULL);
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
#pragma region OPENGL CONFIG-----------------------------------------------
    // Activer le test de profondeur
    glEnable(GL_DEPTH_TEST);

    // Activer le culling des faces
    glEnable(GL_CULL_FACE);

    // Définir l'ordre des vertices pour les faces avant
    // - GL_CCW pour le sens anti horaire (par défaut)
    // - GL_CW pour le sens horaire
    glFrontFace(GL_CCW);

    // Spécifier les faces à cacher
    // - GL_FRONT: les faces-avants
    // - GL_BACK: les faces-arrières (par défaut)
    // - GL_FRONT_AND_BACK: toutes les faces
    // glCullFace(GL_FRONT);

    // Activer la transparence
    glEnable(GL_BLEND);  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Activer la correction gamma
    glEnable(GL_FRAMEBUFFER_SRGB);
#pragma endregion
    // ---------------------------------------------------------------
    Application app;
    // chemins par defaut
    std::string objPath = "assets/teapot/teapot.obj";
    std::string mtlPath = "assets/teapot/";

    // Vérification et chargement des chemins à partir des arguments
    if (argc >= 3) {
        objPath = argv[1];
        mtlPath = argv[2];
    } else {
        std::cout << "Chemin par défaut utilisé car arguments insuffisants" << std::endl;
        std::cout << "Usage: ./opengl_app.exe [chemin_obj] [chemin_mtl]" << std::endl;
        std::cout << "Exemple: ./opengl_app.exe assets/teapot/teapot.obj assets/teapot/" << std::endl;
    }

    app.Initialize(window, WIDTH, HEIGHT, 50, objPath, mtlPath);
    
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