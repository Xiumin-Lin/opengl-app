#include "Application.h"

using namespace std;

#pragma region GLFW WINDOWS CALLBACKS==========================
bool isDragging = false;
bool isFirstClick = true;

void window_resize_callback(GLFWwindow *window, int width, int height)
{
    // cout << "Resized to " << width << "x" << height << endl;
    Application *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
    if (app != nullptr)
    {
        app->ResizeWindow(width, height);
    }
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Application *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
    if (app != nullptr)
    {
        app->GetCamera().scrollCallback(xoffset, yoffset);
    }
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (!isDragging) return;
    Application *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
    if (app != nullptr)
    {
        app->GetCamera().mouseCallback(xpos, ypos, isFirstClick);
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (isFirstClick) {
            isFirstClick = false;
        }
        isDragging = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        isDragging = false;
        isFirstClick = true;
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    Application *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
    if (app != nullptr)
    {
        app->GetCamera().keyCallback(key, scancode, action, mods);
        // Change thee target of the camera to the next mesh
        if(key == GLFW_KEY_F && action == GLFW_PRESS) {
            app->NextMeshFocus();
        }
        // Reset the target of the camera to the origin == UNFOCUS
        else if (key == GLFW_KEY_U && action == GLFW_PRESS) {
            app->GetCamera().setTarget(vec3(0.0f, 0.0f, 0.0f));
        }
    }
}
#pragma endregion

void Application::Initialize(GLFWwindow *window, int width, int height, float cameraInitRadium, const string &object_filename, const string &mtl_basepath)
{
    m_window = window;
    glfwSetWindowUserPointer(window, this); // Stocker le pointeur vers l'instance de la classe Application
    ResizeWindow(width, height);

    glfwSetFramebufferSizeCallback(m_window, window_resize_callback);

    m_camera = CameraOrbitale(vec3(0.0f, 0.0f, 0.0f), cameraInitRadium, 0.0f, 0.0f);
    cout << "Camera Orbitale initialized" << endl;
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetKeyCallback(window, keyCallback);

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
    for (std::unique_ptr<Mesh> &mesh_ptr : m_meshes)
    {
        cout << "> Setup Mesh: " << mesh_ptr->name << endl;
        mesh_ptr->setAttribLocation(
            glGetAttribLocation(program, "a_Position"), // sauvegarder les location index pour 
            glGetAttribLocation(program, "a_Normal"),   // eviter de rappeler glGetAttribLocation à chaque render
            glGetAttribLocation(program, "a_TexCoords")
        );
        mesh_ptr->setUniformLocation(
            glGetUniformLocation(program, "u_Model"),
            glGetUniformLocation(program, "u_NormalMatrix")
        );
        mesh_ptr->generateGLBuffers();   // setup les VBO et IBO en les liant avec les vertices et indices

        // Load texture and specular texture
        Material *material = mesh_ptr->material;
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
    glUniformMatrix4fv(glGetUniformLocation(program, "u_Projection"), 1, GL_FALSE, m_projectionMatrix.data);
#pragma endregion

#pragma region CAMERA VIEW MATRIX
    m_camera.update();
    glUniformMatrix4fv(glGetUniformLocation(program, "u_View"), 1, GL_FALSE, m_camera.getViewMatrix().data);
#pragma endregion

    // glUniform2f(glGetUniformLocation(program, "u_Dimensions"), float(m_windowWidth), float(m_windowHeight));

#pragma region LIGHT-------------------------------------------
    float time = static_cast<float>(glfwGetTime());
    float angle = static_cast<float>(time) * 20.0f;
    float move = sin(static_cast<float>(time));
    GLfloat ambientColor[] = {1.0f, 1.0f, 1.0f}; // Lumière ambiante faible

    GLfloat lightDirection[] = {move, 1.0f, 1.0f}; // Direction de la lumière
    GLfloat lightColor[] = {3.0f, 3.0f, 3.0f};     // Couleur de la source lumineuse

    vec3 camPos = m_camera.getPosition();
    glUniform3f(glGetUniformLocation(program, "u_ViewPosition"), camPos.x, camPos.y, camPos.z);
    glUniform3fv(glGetUniformLocation(program, "u_Light.direction"), 1, lightDirection);
    glUniform3fv(glGetUniformLocation(program, "u_Light.ambientColor"), 1, ambientColor);
    glUniform3fv(glGetUniformLocation(program, "u_Light.diffuseColor"), 1, lightColor);
    glUniform3fv(glGetUniformLocation(program, "u_Light.specularColor"), 1, lightColor);
#pragma endregion

#pragma region DRAW -------------------------------------------

    for (std::unique_ptr<Mesh> &mesh_ptr : m_meshes)
    {
#pragma region WORLD MATRIX = Translate x Rotate x Scale (repecter l ordre)
        Mat4 worldMatrix = Mat4();
        // World Matrix = Translate * Rotate * Scale (dans cet ordre)
        // -------------- Scale ------------------
        worldMatrix.scale(0.01f, 0.01f, 0.01f); // yoda is very big, use this scale and initial radius of the camera to 50
        // -------------- Rotate -----------------
        // worldMatrix.rotateX(angle);
        // worldMatrix.rotateY(angle);
        worldMatrix.rotateZ(angle);
        // -------------- Translate --------------
        // worldMatrix.translate(move, 0.0f, 0.0f);
        mesh_ptr->setWorldMatrix(worldMatrix);
#pragma endregion

        Material *material = mesh_ptr->material;
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
        mesh_ptr->draw();

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

void Application::NextMeshFocus()
{
    if (currentMeshFocusIndex < 0) currentMeshFocusIndex = 0;
    currentMeshFocusIndex = currentMeshFocusIndex % m_meshes.size();
    m_camera.setTarget(m_meshes[currentMeshFocusIndex]->getPosition());
    currentMeshFocusIndex++;
}