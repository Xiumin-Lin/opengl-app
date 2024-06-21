#include "Application.h"
#include <iostream>
#include "Utils.h"
#include "../vendor/stb/stb_image.h"

using namespace std;

void window_resize_callback(GLFWwindow *window, int width, int height)
{
    // cout << "Resized to " << width << "x" << height << endl;
    Application *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
    if (app != nullptr)
    {
        app->ResizeWindow(width, height);
    }
}

void Application::Initialize(GLFWwindow *window, int width, int height, const string &object_filename)
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
    // TEST VAO ============================================================
    // int count = 5;
    // GLuint* VAOs = new GLuint[count];
    // glGenVertexArrays(count, VAOs);
    // for (size_t i = 0; i < count; i++)
    // {
    //     cout << "VAO[" << i << "] = " << VAOs[i] << endl;
    // }

    /**
     * NB: En OpenGL 2.1, VAO est disponible en activant l'option glewExperimental = GL_TRUE 
     * Cependant, il est impossible d'utiliser plusieurs VAOs en même temps
     * Les valeurs de VAOs apres glGenVertexArrays sont les memes et valent 0
     * */
    // Meshes ================================================================
    if (object_filename.empty())
        m_meshes.push_back(std::make_unique<Mesh>(Mesh::GenererRectangle())); // default mesh
    else
        m_meshes = Utils::load_obj(object_filename.c_str());

    if (m_meshes.empty()) { cerr << "No mesh loaded" << endl; exit(1); }
    cout << "Application Loaded " << m_meshes.size() << " meshes" << endl;

    // Comme VAO n'est pas disponible en OpenGL 2.1, on configure et lie en avamce les VBO et IBO
    // puis dans le render, on appelle ConfigRenderParameters pour configurer les attributs de vertex
    const int POSITION = glGetAttribLocation(m_basicProgram.GetProgram(), "a_Position");
    const int NORMAL = glGetAttribLocation(m_basicProgram.GetProgram(), "a_Normal");
    const int TEX_COORD = glGetAttribLocation(m_basicProgram.GetProgram(), "a_TexCoords");
    for (std::unique_ptr<Mesh>& mesh_pt : m_meshes)
    {
        mesh_pt->setAttribLocation(POSITION, NORMAL, TEX_COORD);
        mesh_pt->generateGLBuffers();
    }

    // Texture ===============================================================
    m_textures = new Texture[2];

    if (m_textures[0].Load("./assets/brick.png")) cout << "Texture 1 loaded" << endl;
    else { cerr << "Texture 1 not loaded" << endl; exit(1); }

    if (m_textures[1].Load("./assets/brick-specular.png")) cout << "Texture 2 loaded" << endl;
    else { cerr << "Texture 2 not loaded" << endl; exit(1); }
}

void Application::Render()
{
    glViewport(0, 0, m_windowWidth, m_windowHeight);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    uint32_t program = m_basicProgram.GetProgram();
    glUseProgram(program);

    // Projection -----------------------------------------------
    // compute at each windows resize -> ResizeWindow(int width, int height)

    // Camera View ----------------------------------------------
    m_viewMatrix.loadIdentity();
    m_viewMatrix.translate(0.0f, 0.0f, -40.f);

    // Model  ---------------------------------------------------
    float time = static_cast<float>(glfwGetTime());
    float angle = static_cast<float>(time) * 20.0f;
    // float move = sin(static_cast<float>(time));

    // World Matrix = Translate * Rotate * Scale (dans cet ordre)
    m_modelMatrix.loadIdentity();
    // -------------- Scale ------------------
    // m_modelMatrix.scale(move, move, move);
    // -------------- Rotate -----------------
    m_modelMatrix.rotateX(angle);
    m_modelMatrix.rotateY(angle);
    // -------------- Translate --------------
    // m_modelMatrix.translate(move, 0.0f, 0.0f);

    // Send uniforms Matrix -------------------------------------
    glUniformMatrix4fv(glGetUniformLocation(program, "u_Model"), 1, GL_FALSE, m_modelMatrix.data);
    glUniformMatrix4fv(glGetUniformLocation(program, "u_View"), 1, GL_FALSE, m_viewMatrix.data);
    glUniformMatrix4fv(glGetUniformLocation(program, "u_Projection"), 1, GL_FALSE, m_projectionMatrix.data);

    glUniformMatrix4fv(glGetUniformLocation(program, "u_NormalMatrix"), 1, GL_FALSE, m_modelMatrix.data);

    glUniform2f(glGetUniformLocation(program, "u_Dimensions"), float(m_windowWidth), float(m_windowHeight));

    // Light -----------------------------------------------------
    GLfloat lightColor[] = {1.0f, 1.0f, 1.0f};      // Couleur de la source lumineuse
    GLfloat lightDirection[] = {0.0f, 1.0f, 1.0f};  // Direction de la lumière
    GLfloat ambientColor[] = {0.1f, 0.1f, 0.1f};    // Lumière ambiante faible
    GLfloat diffuseMaterial[] = {1.0f, 1.0f, 1.0f}; // valeur RGB, indique quel pourcentage de RGB est réfléchi par le matériau
    GLfloat cameraPos[] = {m_viewMatrix.data[12], m_viewMatrix.data[13], m_viewMatrix.data[14]};
    GLfloat shininess = 32.0f;

    glUniform3fv(glGetUniformLocation(program, "u_LightColor"), 1, lightColor);
    glUniform3fv(glGetUniformLocation(program, "u_LightDirection"), 1, lightDirection);
    glUniform3fv(glGetUniformLocation(program, "u_AmbientColor"), 1, ambientColor);
    glUniform3fv(glGetUniformLocation(program, "u_DiffuseMaterial"), 1, diffuseMaterial);

    glUniform3fv(glGetUniformLocation(program, "u_ViewPosition"), 1, cameraPos);
    glUniform1f(glGetUniformLocation(program, "u_Shininess"), shininess);

    // Texture ---------------------------------------------------
    m_textures[0].Bind(GL_TEXTURE0, glGetUniformLocation(program, "u_Texture"));
    m_textures[1].Bind(GL_TEXTURE0, glGetUniformLocation(program, "u_Texture1"), 1);

    // Draw ------------------------------------------------------
    for (std::unique_ptr<Mesh>& mesh_pt : m_meshes)
    {
        mesh_pt->draw();
    }
    m_textures[0].Unbind();
    m_textures[1].Unbind();
    // Il on suppose que la phase d'echange des buffers front et back
    // le « swap buffers » est effectuee juste apres
}

void Application::Terminate()
{
    m_meshes.clear();
    delete[] m_textures;
    m_basicProgram.Destroy();
}

void Application::ResizeWindow(int width, int height)
{
    m_windowWidth = width;
    m_windowHeight = height;

    // Recalculer la matrice de projection
    // PERPECTIVE
    Mat4::perspective(&m_projectionMatrix, 45.0f, float(m_windowWidth) / float(m_windowHeight), 0.1f, 100.0f);
    // ORTHO
    // float right = float(m_windowWidth) / 2.0f; // float(m_windowWidth)
    // float top = float(m_windowWidth) / 2.0f;   // float(m_windowHeight)
    // Mat4::ortho(&m_projectionMatrix, -right, right, -top, top, -5.0f, 5.0f);
}