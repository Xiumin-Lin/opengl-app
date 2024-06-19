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
    GLuint VBO, IBO, VAO;

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
        const std::vector<Vertex> triangles = {
            {{0.0f, 0.5f}, {1.0f, 0.0f, 0.0f}},   // rouge
            {{-0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}}, // vert
            {{0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}}   // bleu
        };

        // GLuint == uint32_t
        // GLushort == uint16_t
        const std::vector<uint16_t> indices = { 0, 1, 2 };

        // Générer VBO et IBO
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &IBO);

        // Générer et lier le VAO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferData(GL_ARRAY_BUFFER, triangles.size() * sizeof(Vertex), triangles.data(), GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint16_t), indices.data(), GL_STATIC_DRAW);

            const int POSITION = 0; // glGetAttribLocation(m_basicProgram.GetProgram(), "a_Position");
            const int COLOR = 1;    // glGetAttribLocation(m_basicProgram.GetProgram(), "a_Color");
            glEnableVertexAttribArray(POSITION);
            glVertexAttribPointer(POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

            glEnableVertexAttribArray(COLOR);
            glVertexAttribPointer(COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

        glBindVertexArray(0);
    }

    void Terminate()
    {
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &IBO);
        glDeleteVertexArrays(1, &VAO);
        m_basicProgram.Destroy();
    }

    void Render()
    {
        // etape a. A vous de recuperer/passer les variables width/height
        glViewport(0, 0, WIDTH, HEIGHT);
        // etape b. Notez que glClearColor est un etat, donc persistant
        glClearColor(0.5f, 0.5f, 0.5f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        // etape c. on specifie le shader program a utiliser
        uint32_t program = m_basicProgram.GetProgram();
        glUseProgram(program);

        int timeLocation = glGetUniformLocation(m_basicProgram.GetProgram(), "u_Time");
        double time = glfwGetTime();
        glUniform1f(timeLocation, static_cast<float>(time));

        // etape e. paramétrer le rendu
        glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, nullptr);
        glBindVertexArray(0);

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
    glewExperimental = GL_TRUE; // need for VAO on MacOs
    GLenum error = glewInit();
    if (error != GLEW_OK)
    {
        std::cout << "Erreur d'initialisation de GLEW" << std::endl;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
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
