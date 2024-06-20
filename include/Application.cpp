#include "Application.h"
#include <iostream>
#include "Utils.h"

void window_resize_callback(GLFWwindow *window, int width, int height)
{
    // std::cout << "Resized to " << width << "x" << height << std::endl;
    Application *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
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
        // LoadObject("./assets/cube.obj");
        m_meshes.push_back(Mesh::GenereTriangle());
    }
    else
    {
        LoadObject(object_filename.c_str());
        m_meshes.push_back(Mesh::GenereTriangle());
    }

    if (m_meshes.empty())
    {
        std::cerr << "No mesh loaded" << std::endl;
        exit(1);
    }

    // Comme VAO n'est pas disponible en OpenGL 2.1, on configure et lie en avamce les VBO et IBO
    // puis dans le render, on appelle ConfigRenderParameters pour configurer les attributs de vertex
    const int POSITION = glGetAttribLocation(m_basicProgram.GetProgram(), "a_Position");
    const int NORMAL = glGetAttribLocation(m_basicProgram.GetProgram(), "a_Normal");
    const int TEX_COORD = glGetAttribLocation(m_basicProgram.GetProgram(), "a_TexCoords");
    for (Mesh &mesh : m_meshes)
    {
        mesh.GenerateGLBuffers();
        mesh.SetLocation(POSITION, NORMAL, TEX_COORD);
    }
}

void Application::Render()
{
    glViewport(0, 0, m_windowWidth, m_windowHeight);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_basicProgram.GetProgram());

    // Projection -----------------------------------------------
    // compute at each windows resize -> ResizeWindow(int width, int height)

    // Camera View -----------------------------------------------------
    m_viewMatrix.loadIdentity();
    m_viewMatrix.translate(0.0f, 0.0f, -4.f);

    // Model Transformation -------------------------------------
    float time = static_cast<float>(glfwGetTime());
    float angle = static_cast<float>(time) * 40.0f;
    // float move = std::sin(static_cast<float>(time));

    m_modelMatrix.loadIdentity();
    // m_modelMatrix.scale(move, move, 1);
    // m_modelMatrix.translate(move, 0.0f, 0.0f);
    m_modelMatrix.rotateY(angle);

    // Send uniforms --------------------------------------------
    glUniformMatrix4fv(glGetUniformLocation(m_basicProgram.GetProgram(), "u_Model"), 1, GL_FALSE, m_modelMatrix.data);
    glUniformMatrix4fv(glGetUniformLocation(m_basicProgram.GetProgram(), "u_View"), 1, GL_FALSE, m_viewMatrix.data);
    glUniformMatrix4fv(glGetUniformLocation(m_basicProgram.GetProgram(), "u_Projection"), 1, GL_FALSE, m_projectionMatrix.data);

    glUniform2f(glGetUniformLocation(m_basicProgram.GetProgram(), "u_Dimensions"), float(m_windowWidth), float(m_windowHeight));

    // Draw -----------------------------------------------------
    for (Mesh &mesh : m_meshes)
    {
        // VBO and IBO are binded in ConfigRenderParameters
        mesh.ConfigRenderParameters();

        glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, nullptr);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    // Il on suppose que la phase d'echange des buffers front et back
    // le « swap buffers » est effectuee juste apres
}

void Application::Terminate()
{
    for (Mesh &mesh : m_meshes)
    {
        mesh.DeleteGLBuffers();
    }
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
    // PERPECTIVE
    m_projectionMatrix = Mat4::perspective(45.0f, float(m_windowWidth) / float(m_windowHeight), 0.1f, 100.0f);
    // ORTHO GENERIQUE
    // m_projectionMatrix = Mat4::ortho(0.0f, float(m_windowWidth), float(m_windowHeight), 0.0f, -1.0f, 1.0f);
}