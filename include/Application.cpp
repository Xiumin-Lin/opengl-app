#include "Application.h"
#include <iostream>
#include "Utils.h"

void Application::Initialize(int width, int height)
{
    m_width = width;
    m_height = height;
    m_basicProgram.LoadVertexShader("./shaders/basic.vs.glsl");
    m_basicProgram.LoadFragmentShader("./shaders/basic.fs.glsl");
    m_basicProgram.Create();

    // cette fonction est spécifique à Windows et permet d’activer (1) ou non (0)
    // la synchronization vertical. Elle necessite l’include wglew.h
#ifdef WIN32
    wglSwapIntervalEXT(1);
#endif

    std::vector<Vertex> triangles = {
        {{0.0f, 0.5f}, {1.0f, 0.0f, 0.0f}},   // rouge
        {{-0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}}, // vert
        {{0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}}   // bleu
    };

    // GLuint == uint32_t // GLushort == uint16_t
    std::vector<uint16_t> indices = {0, 1, 2};

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

    int POSITION = 0; // glGetAttribLocation(m_basicProgram.GetProgram(), "a_Position");
    glEnableVertexAttribArray(POSITION);
    glVertexAttribPointer(POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));

    // int COLOR = 1; // glGetAttribLocation(m_basicProgram.GetProgram(), "a_Color");
    // glEnableVertexAttribArray(COLOR);
    // glVertexAttribPointer(COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));

    glBindVertexArray(0);
}

void Application::Render()
{
    glViewport(0, 0, m_width, m_height);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_basicProgram.GetProgram());

    glUniform1f(glGetUniformLocation(m_basicProgram.GetProgram(), "u_Time"), static_cast<float>(glfwGetTime()));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, nullptr);
    glBindVertexArray(0);

    // Il on suppose que la phase d'echange des buffers front et back
    // le « swap buffers » est effectuee juste apres
}

void Application::Terminate()
{
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
    glDeleteVertexArrays(1, &VAO);
    m_basicProgram.Destroy();
}

void Application::LoadObject(const char *filename)
{
    m_meshes = Utils::load_obj(filename);
    std::cout << "Application Loaded " << m_meshes.size() << " meshes" << std::endl;
}