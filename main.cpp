#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <cmath>

#include "./common/GLShader.h"

const int WIDTH = 800;
const int HEIGHT = 600;

struct vec2 { float x, y; };
struct vec3 { float x, y, z; };
struct vec4 { float x, y, z, w; };

struct Point
{
    vec2 position;
};

struct Color
{
    vec3 rgb;
};

struct Application
{
    GLShader g_BasicShader;
    bool Initialize()
    {
        g_BasicShader.LoadVertexShader("./shaders/basic.vs");
        g_BasicShader.LoadFragmentShader("./shaders/basic.fs");
        g_BasicShader.Create();

    // cette fonction est spécifique à Windows et permet d’activer (1) ou non (0)
    // la synchronization vertical. Elle necessite l’include wglew.h
#ifdef WIN32
        wglSwapIntervalEXT(1);
#endif
        return true;
    }

    void Terminate()
    {
        g_BasicShader.Destroy();
    }

    void Render()
    {
        // etape a. A vous de recuperer/passer les variables width/height
        glViewport(0, 0, WIDTH, HEIGHT);

        // etape b. Notez que giClearColor est un etat, donc persistant
        glClearColor(0.5f, 0.5f, 0.5f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        // etape c. on specifie le shader program a utiliser
        auto basicProgram = g_BasicShader.GetProgram();
        glUseProgram(basicProgram);

        // etape d. définition d’une géométrie
        // premier parametre = 0, correspond ici au canal/emplacement du premier attribut
        // glEnableVertexAttribArray() indique que les donnees sont generiques (proviennent
        // d’un tableau) et non pas communes a tous les sommets
        const Point triangle[] = {
            {{-0.5f, -0.5f}},
            {{0.5f, -0.5f}},
            {{0.0f, 0.5f}}
        };

        int loc_position = glGetAttribLocation(basicProgram, "a_Position");
        glEnableVertexAttribArray(loc_position);
        // glVertexAttribPointer(index, taille, type, normalisation, écart, adresse)
        glVertexAttribPointer(loc_position, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, triangle);

        // Exercice A.1 - Colorier le triangle
        const Color color[] = {
            {{1.f, 1.f, 0.f}},
            {{1.f, 0.f, 1.f}},
            {{0.f, 1.f, 1.f}}
        };

        int loc_color = glGetAttribLocation(basicProgram, "a_Color");
        glEnableVertexAttribArray(loc_color);
        glVertexAttribPointer(loc_color, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, color);


        // etape e. paramétrer le rendu

        // etape f. dessin de triangles dont la definition provient d'un tableau
        // le rendu s'effectue ici en prenant 3 sommets a partir du debut du tableau (0)
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Il on suppose que la phase d'echange des buffers front et back
        // le « swap buffers » est effectuee juste apres
    }
};

int main(void)
{
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "Exemple OpenGL 2.+", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // ICI !
    GLenum error = glewInit();
    if (error != GLEW_OK)
    {
        std::cout << "Erreur d'initialisation de GLEW" << std::endl;
    }

    Application app;
    app.Initialize();

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
