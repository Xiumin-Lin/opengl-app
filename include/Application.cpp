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

void Application::Initialize(GLFWwindow *window, int width, int height, const string &object_filename, const string &mtl_basepath)
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
        m_meshes = Utils::load_obj(object_filename.c_str(), mtl_basepath.c_str());

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
    m_texture = new Texture();

    if (m_texture->Load("./assets/brick.png")) cout << "Texture 1 loaded" << endl;
    else { cerr << "Texture 1 not loaded" << endl; exit(1); }

    m_specular_texture = new Texture();
    if (m_specular_texture->Load("./assets/brick-specular.png")) cout << "Specular Texture loaded" << endl;
    else { cerr << "Specular Texture not loaded" << endl; exit(1); }
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
    GLfloat ambientColor[] = {0.5f, 0.5f, 0.5f};    // Lumière ambiante faible
    GLfloat cameraPos[] = {m_viewMatrix.data[12], m_viewMatrix.data[13], m_viewMatrix.data[14]};

    GLfloat lightDirection[] = {0.0f, 1.0f, 1.0f};  // Direction de la lumière
    GLfloat lightColor[] = {0.3f, 0.9f, 0.7f};      // Couleur de la source lumineuse

    GLfloat matAmbient[] = {0.0f, 0.0f, 1.0f};
    GLfloat matDiffuse[] = {0.4f, 0.6f, 0.8f}; // valeur RGB, indique quel pourcentage de RGB est réfléchi par le matériau
    GLfloat matSpecular[] = {0.4f, 0.6f, 0.8f};
    GLfloat shininess = 8.0f;

    glUniform3fv(glGetUniformLocation(program, "u_ViewPosition"), 1, cameraPos);
    
    glUniform3fv(glGetUniformLocation(program, "u_Light.direction"), 1, lightDirection);
    glUniform3fv(glGetUniformLocation(program, "u_Light.ambientColor"), 1, ambientColor);
    glUniform3fv(glGetUniformLocation(program, "u_Light.diffuseColor"), 1, lightColor);
    glUniform3fv(glGetUniformLocation(program, "u_Light.specularColor"), 1, lightColor);

    glUniform3fv(glGetUniformLocation(program, "u_Material.ambientColor"), 1, matAmbient);
    glUniform3fv(glGetUniformLocation(program, "u_Material.diffuseColor"), 1, matDiffuse);
    glUniform3fv(glGetUniformLocation(program, "u_Material.specularColor"), 1, matSpecular);
    glUniform1f(glGetUniformLocation(program, "u_Material.shininess"), shininess);

    // Texture ---------------------------------------------------
    m_texture->Bind(GL_TEXTURE0, glGetUniformLocation(program, "u_Texture"));
    m_specular_texture->Bind(GL_TEXTURE0, glGetUniformLocation(program, "u_Texture_Specular"), 1);

    // Draw ------------------------------------------------------
    for (std::unique_ptr<Mesh>& mesh_pt : m_meshes)
    {
        mesh_pt->draw();
    }
    m_texture->Unbind();
    m_specular_texture->Unbind();
    // Il on suppose que la phase d'echange des buffers front et back
    // le « swap buffers » est effectuee juste apres
}

void Application::Terminate()
{
    m_meshes.clear();
    delete m_texture;
    delete m_specular_texture;
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