#include "Application.h"
#include <iostream>
#include "Utils.h"

void window_resize_callback(GLFWwindow *window, int width, int height)
{
    // std::cout << "Resized to " << width << "x" << height << std::endl;
    Application* app = static_cast<Application *>(glfwGetWindowUserPointer(window));
    if (app != nullptr)
    {
        app->ResizeWindow(width, height);
    }
}

void Application::Initialize(GLFWwindow *window, int width, int height, const std::string &object_filename)
{
    m_window = window;
    glfwSetWindowUserPointer(window, this); // Stocker le pointeur vers l'instance de la classe Application
    ResizeWindow(width, height);
    glfwSetFramebufferSizeCallback(m_window, window_resize_callback);

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
    glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));

    const int NORMAL = glGetAttribLocation(m_basicProgram.GetProgram(), "a_Normal");
    if (NORMAL >= 0)
    {
        glEnableVertexAttribArray(NORMAL);
        glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
    }

    const int TEX_COORD = glGetAttribLocation(m_basicProgram.GetProgram(), "a_TexCoords");
    if (TEX_COORD >= 0)
    {
        glEnableVertexAttribArray(TEX_COORD);
        glVertexAttribPointer(TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texcoords));
    }
    glBindVertexArray(0);
}

void Application::Render()
{
    glViewport(0, 0, m_windowWidth, m_windowHeight);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_basicProgram.GetProgram());

    // Projection -----------------------------------------------
    // compute at each windows resize -> ResizeWindow(int width, int height)

    // Camera View -----------------------------------------------------
    m_viewMatrix.loadIdentity();
    m_viewMatrix.translate(0.0f, 0.0f, -3.f);

    // Model Transformation -------------------------------------
    float time = static_cast<float>(glfwGetTime());
    // float move = std::sin(static_cast<float>(time)) * 100.0f;
    float angle = static_cast<float>(time) * 40.0f;

    m_modelMatrix.loadIdentity();
    m_modelMatrix.scale(0.5f, 0.5f, 0.5f);
    // m_modelMatrix.translate(100.0f, 0.0f, 0.0f);
    m_modelMatrix.rotateY(angle);

    // Send uniforms --------------------------------------------
    glUniformMatrix4fv(glGetUniformLocation(m_basicProgram.GetProgram(), "u_Model"), 1, GL_FALSE, m_modelMatrix.data);
    glUniformMatrix4fv(glGetUniformLocation(m_basicProgram.GetProgram(), "u_View"), 1, GL_FALSE, m_viewMatrix.data);
    glUniformMatrix4fv(glGetUniformLocation(m_basicProgram.GetProgram(), "u_Projection"), 1, GL_FALSE, m_projectionMatrix.data);

    // Draw -----------------------------------------------------
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
    m_windowWidth = width;
    m_windowHeight = height;

    // Recalculer la matrice de projection
    m_projectionMatrix = Mat4::perspective(45.0f, float(m_windowWidth) / float(m_windowHeight), 0.1f, 100.0f);
    // m_projectionMatrix = Mat4::ortho(0.0f, float(m_windowWidth), float(m_windowHeight), 0.0f, -1.0f, 1.0f);
}