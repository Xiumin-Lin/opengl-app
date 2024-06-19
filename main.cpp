#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <cmath>

#include "common/GLShader.h"
const int WIDTH = 800;
const int HEIGHT = 600;
struct vec2
{
    float x, y;
};
struct vec3
{
    float x, y, z;
};
struct vec4
{
    float x, y, z, w;
};

struct Vertex
{
    vec2 position;
    vec3 color;
};

struct Application
{
    GLShader m_basicProgram;

    void Initialize()
    {
        m_basicProgram.LoadVertexShader("./shaders/basic.vs.glsl");
        m_basicProgram.LoadFragmentShader("./shaders/basic.fs.glsl");
        m_basicProgram.Create();
        // cette fonction est spécifique à Windows et permet d’activer (1) ou non (0)
        // la synchronization vertical. Elle necessite l’include wglew.h
#ifdef WIN32
        wglSwapIntervalEXT(1);
#endif
    }

    void Terminate()
    {
        m_basicProgram.Destroy();
    }

    void Render()
    {
        // etape a. A vous de recuperer/passer les variables width/height
        glViewport(0, 0, WIDTH, HEIGHT);

        // etape b. Notez que giClearColor est un etat, donc persistant
        glClearColor(0.5f, 0.5f, 0.5f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        // etape c. on specifie le shader program a utiliser
        uint32_t program = m_basicProgram.GetProgram();
        glUseProgram(program);

        // etape d. définition d’une géométrie
        // premier parametre = 0, correspond ici au canal/emplacement du premier attribut
        // glEnableVertexAttribArray() indique que les donnees sont generiques (proviennent
        // d’un tableau) et non pas communes a tous les sommets
        const std::vector<Vertex> triangles =
            {
                {{0.0f, 0.5f}, {1.0f, 0.0f, 0.0f}},   // sommet 0 et couleur rouge
                {{-0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}}, // sommet 1 et couleur verte
                {{0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}}   // sommet 2 et couleur bleue
            };

        // {[x, y], [r, g, b]}
        const int POSITION = glGetAttribLocation(program, "a_Position");
        const int COLOR = glGetAttribLocation(program, "a_Color");
        if (POSITION < 0 || COLOR < 0)
        {
            std::cout << "erreur de programme";
            return;
        }

        glEnableVertexAttribArray(POSITION);
        // &triangles[0].position est un pointeur vers le premier élément de la première position dans le tableau triangles.
        // ensuite chaque sommet est composé de 2 GL_FLOAT et ne sont pas normalisées (GL_FALSE)
        // et que le stride (l’espacement entre les attributs successifs) est de sizeof(Vertex) (taille d'un sommet)
        glVertexAttribPointer(POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex) /*stride*/, &triangles[0].position);

        glEnableVertexAttribArray(COLOR);
        glVertexAttribPointer(COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex) /*stride*/, &triangles[0].color);

        // etape e. paramétrer le rendu
        int timeLocation = glGetUniformLocation(program, "u_Time");
        double time = glfwGetTime();
        glUniform1f(timeLocation, static_cast<float>(time));

        // etape f. dessin de triangles dont la definition provient d'un tableau
        // le rendu s'effectue ici en prenant 3 sommets a partir du debut du tableau (0)
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // GLuint == uint32_t
        // GLushort == uint16_t
        //  const uint16_t indices[] = { 0, 1, 2 };
        // glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, indices);

        glDisableVertexAttribArray(POSITION);
        glDisableVertexAttribArray(COLOR);

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
