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
            app->NextObjectFocus();
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
    // Stocker le pointeur vers l'instance de la classe Application
    // Utiliser pour les callbacks de GLFW
    glfwSetWindowUserPointer(window, this);
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
    // Charger l'objet par défaut
    Object defaultObject;
    defaultObject.loadMeshes("./assets/teapot/teapot.obj", "./assets/teapot/");
    m_objects.push_back(std::move(defaultObject));

    // Charger l'objet spécifié par le param
    Object userObject;
    if (!object_filename.empty()) {
        userObject.loadMeshes(object_filename, mtl_basepath);
    } else {
        userObject.loadMeshes("./assets/teapot/teapot.obj", "./assets/teapot/");
    }
    m_objects.push_back(std::move(userObject));

    cout << "Application Loaded :" << m_objects.size() << " object" << endl;
#pragma endregion

#pragma region CONFIGURE MESHES================================
    uint32_t program = m_basicProgram.GetProgram();
    for (Object &object : m_objects)
    {
        object.setupMeshes(program);
    }
#pragma endregion

}

void Application::Render()
{
    glViewport(0, 0, m_windowWidth, m_windowHeight);
    glClearColor(0.7f, 0.7f, 0.7f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    uint32_t program = m_basicProgram.GetProgram();
    glUseProgram(program);

#pragma region PROJECTION MATRIX-------------------------------
    // compute at each windows resize -> ResizeWindow(int width, int height)
    glUniformMatrix4fv(glGetUniformLocation(program, "u_Projection"), 1, GL_FALSE, m_projectionMatrix.data);
#pragma endregion

#pragma region CAMERA VIEW MATRIX------------------------------
    m_camera.update();
    glUniformMatrix4fv(glGetUniformLocation(program, "u_View"), 1, GL_FALSE, m_camera.getViewMatrix().data);
#pragma endregion

    // glUniform2f(glGetUniformLocation(program, "u_Dimensions"), float(m_windowWidth), float(m_windowHeight));

#pragma region LIGHT-------------------------------------------
    float time = static_cast<float>(glfwGetTime());
    float light_move = sin(static_cast<float>(time));

    GLfloat ambientColor[] = {1.0f, 1.0f, 1.0f};    // Lumiere ambiante faible
    GLfloat lightDirection[] = {light_move, 0.0f, 2.0f};  // Direction de la lumiere
    GLfloat lightColor[] = {3.0f, 3.0f, 3.0f};      // Couleur de la lumiere

    vec3 camPos = m_camera.getPosition();
    glUniform3f(glGetUniformLocation(program, "u_ViewPosition"), camPos.x, camPos.y, camPos.z);
    glUniform3fv(glGetUniformLocation(program, "u_Light.direction"), 1, lightDirection);
    glUniform3fv(glGetUniformLocation(program, "u_Light.ambientColor"), 1, ambientColor);
    glUniform3fv(glGetUniformLocation(program, "u_Light.diffuseColor"), 1, lightColor);
    glUniform3fv(glGetUniformLocation(program, "u_Light.specularColor"), 1, lightColor);

    // ILLUMINATION HEMISPHERE
    // vec3 skyColor = vec3(0.0, 0.1, 0.4);        // Bleu clair pour le ciel
    // vec3 groundColor = vec3(0.3, 0.25, 0.2);    // Marron pour le sol
    // vec3 skyDirection = vec3(0.0, 1.0, 0.0);    // Direction du ciel
    // vec3 ambientIntensity = vec3(0.1, 0.1, 0.1);// Intensité lumineuse ambiante faible

    // glUniform3f(glGetUniformLocation(m_basicProgram.GetProgram(), "u_SkyColor"), skyColor.x, skyColor.y, skyColor.z);
    // glUniform3f(glGetUniformLocation(m_basicProgram.GetProgram(), "u_GroundColor"), groundColor.x, groundColor.y, groundColor.z);
    // glUniform3f(glGetUniformLocation(m_basicProgram.GetProgram(), "u_SkyDirection"), skyDirection.x, skyDirection.y, skyDirection.z);
    // glUniform3f(glGetUniformLocation(m_basicProgram.GetProgram(), "u_LightAmbientIntensity"), ambientIntensity.x, ambientIntensity.y, ambientIntensity.z);
#pragma endregion

#pragma region WORLD MATRIX = Translate x Rotate x Scale (repecter l ordre)
        // World Matrix = Translate * Rotate * Scale (dans l'ordre: scale > rotate > translate)
        Mat4 worldMatrix = Mat4();
        float scale = sin(static_cast<float>(time)) * 0.5f + 0.5f;  // 0.5 < scale < 1.0
        float angle = static_cast<float>(time) * 20.0f;
        float move = sin(static_cast<float>(time)) * 10.0f;
        // -------------- Scale ------------------
        worldMatrix.scale(scale, scale, scale);
        // -------------- Rotate -----------------
        worldMatrix.rotateX(-90);
        // worldMatrix.rotateY(worldMatrix_angle);
        worldMatrix.rotateZ(angle);
        // -------------- Translate --------------
        worldMatrix.translate(30.0f, 0.0f, move); // sur le coté et en mouvement de haut en bas

        Mat4 worldMatrix_yoda = Mat4();
        // -------------- Scale ------------------
        worldMatrix_yoda.scale(0.005f, 0.005f, 0.005f); // yoda is very big, active this scale and initial radius of the camera to 50
        // -------------- Rotate -----------------
        worldMatrix_yoda.rotateX(-90);
        worldMatrix_yoda.rotateZ(-angle);
        // -------------- Translate --------------
        // worldMatrix_yoda.translate(0.0f, 0.0f, 0.0f);
#pragma endregion

#pragma region DRAW -------------------------------------------
    for (size_t i = 0; i < m_objects.size(); i++)
    {
        Object &object = m_objects[i];
        // DEBUG on suppose que l'objet yoda est le deuxieme pour les tests
        bool isObjectYoda = i == 1 ? true : false;
        for (std::unique_ptr<Mesh> &mesh_ptr : object.getMeshes())
        {
            // Set World Matrix -------------------------------
            mesh_ptr->setWorldMatrix(isObjectYoda ? worldMatrix_yoda : worldMatrix);

            // SETUP Material light ---------------------------
            Material *material = mesh_ptr->material;
            material->configUniformMaterialParameters();
            // Bind Texture if it exists ----------------------
            glUniform1i(glGetUniformLocation(program, "u_Material.hasTexture"), material->has_texture ? 1 : 0);
            if (material->has_texture)
                material->texture->bind(GL_TEXTURE0, glGetUniformLocation(program, "u_Texture"), 0);

            glUniform1i(glGetUniformLocation(program, "u_Material.hasTextureSpecular"), material->has_specular_texture ? 1 : 0);
            if (material->has_specular_texture)
                material->specular_texture->bind(GL_TEXTURE0, glGetUniformLocation(program, "u_TextureSpecular"), 1);

            // Draw mesh with material ------------------------
            mesh_ptr->draw();

            // Unbind Texture ---------------------------------
            material->unbindTexture();
        }
    }
#pragma endregion
    // Il on suppose que la phase d'echange des buffers front et back
    // le « swap buffers » est effectuee juste apres
}

void Application::Terminate()
{
    for (Object &object : m_objects)
    {
        object.m_meshes.clear();
    }
    m_objects.clear();
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

void Application::NextObjectFocus()
{
    if (currentObjectFocusIndex < 0) currentObjectFocusIndex = 0;
    currentObjectFocusIndex = currentObjectFocusIndex % m_objects.size();
    m_camera.setTarget(m_objects[currentObjectFocusIndex].getPosition());
    currentObjectFocusIndex++;
}