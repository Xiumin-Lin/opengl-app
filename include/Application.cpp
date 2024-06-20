#include "Application.h"
#include <iostream>
#include "Utils.h"

int Application::s_windowWidth = 800;
int Application::s_windowHeight = 600;

void Application::Initialize(int width, int height, const std::string &object_filename)
{
    Application::ResizeWindow(width, height);
    m_basicProgram.LoadVertexShader("./shaders/basic.vs.glsl");
    m_basicProgram.LoadFragmentShader("./shaders/basic.fs.glsl");
    m_basicProgram.Create();

    // cette fonction est spécifique à Windows et permet d’activer (1) ou non (0)
    // la synchronization vertical. Elle necessite l’include wglew.h
#ifdef WIN32
    wglSwapIntervalEXT(1);
#endif

    if (object_filename.empty())
    {
        Mesh triangleMesh;
        triangleMesh.vertexCount = 3;

        Vertex v1 = {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}};
        Vertex v2 = {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}};
        Vertex v3 = {{0.0f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}};
        triangleMesh.vertices.push_back(v1);
        triangleMesh.vertices.push_back(v2);
        triangleMesh.vertices.push_back(v3);

        m_meshes.push_back(triangleMesh);
    }
    else
    {
        LoadObject(object_filename.c_str());
        if (m_meshes.empty())
        {
            std::cerr << "No mesh loaded" << std::endl;
            exit(1);
        }
    }

    // gen & bind VBO
    glGenBuffers(1, &VBO);
    // gen & bind VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // TODO: seulement un mesh pour l'instant
        glBufferData(GL_ARRAY_BUFFER, m_meshes[0].vertexCount * sizeof(Vertex), m_meshes[0].vertices.data(), GL_STATIC_DRAW);

        const int POSITION = glGetAttribLocation(m_basicProgram.GetProgram(), "a_Position");
        glEnableVertexAttribArray(POSITION);
        glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

        const int NORMAL = glGetAttribLocation(m_basicProgram.GetProgram(), "a_Normal");
        if (NORMAL >= 0) {
            glEnableVertexAttribArray(NORMAL);
            glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        }

        const int TEX_COORD = glGetAttribLocation(m_basicProgram.GetProgram(), "a_TexCoords");
        if (TEX_COORD >= 0) {
            glEnableVertexAttribArray(TEX_COORD);
            glVertexAttribPointer(TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoords));
        }
    glBindVertexArray(0);
}

void Application::Render()
{
    glViewport(0, 0, s_windowWidth, s_windowHeight);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_basicProgram.GetProgram());

    // --------------------------------------------------------------

    // Calculer la rotation et la translation basées sur le temps
    float time = static_cast<float>(glfwGetTime());
    // float move = std::sin(static_cast<float>(time)) * 0.5f; // Oscille entre -0.5 et 0.5
    float angle = static_cast<float>(time) * 50.0f; // Rotation en degrés par seconde

    m_meshMatrix.loadIdentity();
    // m_meshMatrix.translate(move, 0.0f, 0.0f);   // translation en X
    m_meshMatrix.rotateZ(angle);                // rotation autour de l'axe Z

    GLint modelLocation = glGetUniformLocation(m_basicProgram.GetProgram(), "u_Model");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, m_meshMatrix.data);

    // --------------------------------------------------------------

    glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, m_meshes[0].vertexCount);
    // glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, nullptr);
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

void Application::ResizeWindow(int width, int height)
{
    Application::s_windowWidth = width;
    Application::s_windowHeight = height;
}