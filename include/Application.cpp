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
#pragma region TEST VAO in OpenGL 2.1
    /**
     * NB: En OpenGL 2.1, VAO est disponible en activant l'option glewExperimental = GL_TRUE
     * Cependant, il est impossible d'utiliser plusieurs VAOs en même temps
     * Les valeurs de VAOs apres glGenVertexArrays sont les memes et valent 0
     * */
    // int count = 5;
    // GLuint* VAOs = new GLuint[count];
    // glGenVertexArrays(count, VAOs);
    // for (size_t i = 0; i < count; i++)
    // {
    //     cout << "VAO[" << i << "] = " << VAOs[i] << endl;
    // }
#pragma endregion
    
#pragma region MESHES LOADING==================================
    if (object_filename.empty()) m_meshes.push_back(std::make_unique<Mesh>(Mesh::GenererRectangle())); // default mesh
    else m_meshes = Utils::load_obj(object_filename, mtl_basepath);

    if (m_meshes.empty()) { cerr << "No mesh loaded" << endl; exit(1); }
    cout << "Application Loaded " << m_meshes.size() << " meshes" << endl;
#pragma endregion

#pragma region CONFIGURE MESHES================================
    // Comme VAO n'est pas disponible en OpenGL 2.1, on configure et lie en avamce les VBO et IBO
    // puis dans le render, on appelle ConfigRenderParameters pour configurer les attributs de vertex
    int program = m_basicProgram.GetProgram();
    for (std::unique_ptr<Mesh> &mesh_pt : m_meshes)
    {
        cout << "> Setup Mesh: " << mesh_pt->name << endl;
        mesh_pt->setAttribLocation(
            glGetAttribLocation(program, "a_Position"), // sauvegarder les location index pour 
            glGetAttribLocation(program, "a_Normal"),   // eviter de rappeler glGetAttribLocation à chaque render
            glGetAttribLocation(program, "a_TexCoords")
        );
        mesh_pt->generateGLBuffers();   // setup les VBO et IBO en les liant avec les vertices et indices

        // Load texture and specular texture
        Material *material = mesh_pt->material;
        material->setMaterialAttribLocation(    // sauvegarder les location index
            glGetUniformLocation(program, "u_Material.ambientColor"), 
            glGetUniformLocation(program, "u_Material.diffuseColor"), 
            glGetUniformLocation(program, "u_Material.specularColor"),
            glGetUniformLocation(program, "u_Material.shininess")
        );
        material->tryLoadTexture();         // tente de load les textures
        material->tryLoadSpecularTexture(); // (le nom de des textures est recuperé dans le fichier mtl via tiny_obj_loader)
        cout << endl;                       // si succes, les textures sont bind et has_texture est mis à true
    }
#pragma endregion
}

void Application::Render()
{
    glViewport(0, 0, m_windowWidth, m_windowHeight);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    uint32_t program = m_basicProgram.GetProgram();
    glUseProgram(program);

#pragma region PROJECTION MATRIX-------------------------------
    // compute at each windows resize -> ResizeWindow(int width, int height)
#pragma endregion

#pragma region CAMERA VIEW MATRIX------------------------------
    m_viewMatrix.loadIdentity();
    m_viewMatrix.translate(0.0f, 0.0f, -40.f);
#pragma endregion

#pragma region MODEL MATRIX-------------------------------------
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
#pragma endregion 

#pragma region SEND UNIFORM MATRIX-----------------------------
    glUniformMatrix4fv(glGetUniformLocation(program, "u_Model"), 1, GL_FALSE, m_modelMatrix.data);
    glUniformMatrix4fv(glGetUniformLocation(program, "u_View"), 1, GL_FALSE, m_viewMatrix.data);
    glUniformMatrix4fv(glGetUniformLocation(program, "u_Projection"), 1, GL_FALSE, m_projectionMatrix.data);

    glUniformMatrix4fv(glGetUniformLocation(program, "u_NormalMatrix"), 1, GL_FALSE, m_modelMatrix.data);

    // glUniform2f(glGetUniformLocation(program, "u_Dimensions"), float(m_windowWidth), float(m_windowHeight));
#pragma endregion

#pragma region LIGHT ------------------------------------------
    GLfloat ambientColor[] = {0.5f, 0.5f, 0.5f}; // Lumière ambiante faible
    GLfloat cameraPos[] = {m_viewMatrix.data[12], m_viewMatrix.data[13], m_viewMatrix.data[14]};

    GLfloat lightDirection[] = {0.0f, 1.0f, 1.0f}; // Direction de la lumière
    GLfloat lightColor[] = {1.0f, 1.f, 1.0f};     // Couleur de la source lumineuse

    glUniform3fv(glGetUniformLocation(program, "u_ViewPosition"), 1, cameraPos);

    glUniform3fv(glGetUniformLocation(program, "u_Light.direction"), 1, lightDirection);
    glUniform3fv(glGetUniformLocation(program, "u_Light.ambientColor"), 1, ambientColor);
    glUniform3fv(glGetUniformLocation(program, "u_Light.diffuseColor"), 1, lightColor);
    glUniform3fv(glGetUniformLocation(program, "u_Light.specularColor"), 1, lightColor);
#pragma endregion

#pragma region DRAW -------------------------------------------
    for (std::unique_ptr<Mesh> &mesh_pt : m_meshes)
    {
        Material *material = mesh_pt->material;
        // SETUP Material light -------------------------------
        material->configUniformMaterialParameters();
        // Bind Texture if present ----------------------------
        glUniform1i(glGetUniformLocation(program, "u_Material.hasTexture"), material->has_texture ? 1 : 0);
        if (material->has_texture)
            material->texture->bind(GL_TEXTURE0, glGetUniformLocation(program, "u_Texture"), 0);

        glUniform1i(glGetUniformLocation(program, "u_Material.hasTextureSpecular"), material->has_specular_texture ? 1 : 0);
        if (material->has_specular_texture)
            material->specular_texture->bind(GL_TEXTURE0, glGetUniformLocation(program, "u_TextureSpecular"), 1);

        // Draw mesh with material ----------------------------
        mesh_pt->draw();

        // Unbind Texture -------------------------------------
        material->unbindTexture();
    }
#pragma endregion
    // Il on suppose que la phase d'echange des buffers front et back
    // le « swap buffers » est effectuee juste apres
}

void Application::Terminate()
{
    m_meshes.clear();
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