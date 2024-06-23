#include "Object.h"
#include "Utils.h"
#include <iostream>

using namespace std;

void Object::loadMeshes(const string& object_filename, const string& mtl_basepath) {
    m_meshes = Utils::load_obj(object_filename, mtl_basepath);
    if (m_meshes.empty()) {
        cerr << "No meshes loaded." << endl;
        exit(1);
    } else {
        cout << "Loaded " << m_meshes.size() << " meshes." << endl;
    }
}

/**
 * Setup les attributs de vertex et les uniforms pour chaque mesh de l'object
 * Genere et bind les VBO et IBO pour chaque mesh
 * Setup les uniforms pour le material de chaque mesh
 * Charge les textures et specular textures si elles existent
 */
void Object::setupMeshes(uint32_t program) {
    for (unique_ptr<Mesh>& mesh : m_meshes) {
        cout << "> Setup Mesh: " << mesh->name << endl;
        mesh->setAttribLocation(
            glGetAttribLocation(program, "a_Position"), // sauvegarder les location index pour 
            glGetAttribLocation(program, "a_Normal"),   // eviter de rappeler glGetAttribLocation à chaque render
            glGetAttribLocation(program, "a_TexCoords")
        );
        mesh->setUniformLocation(
            glGetUniformLocation(program, "u_Model"),
            glGetUniformLocation(program, "u_NormalMatrix")
        );

        // Comme VAO n'est pas disponible en OpenGL 2.1, on configure et lie en avance les VBO et IBO
        // puis dans le render, on appelle ConfigRenderParameters pour configurer les attributs de vertex
        mesh->generateGLBuffers();  // setup les VBO et IBO en les liant avec les vertices et indices

        // Load texture and specular texture
        mesh->material->setMaterialAttribLocation(
            glGetUniformLocation(program, "u_Material.ambientColor"), 
            glGetUniformLocation(program, "u_Material.diffuseColor"), 
            glGetUniformLocation(program, "u_Material.specularColor"),
            glGetUniformLocation(program, "u_Material.shininess")
        );

        // tente de load les textures
        // (le nom de des textures est recuperé dans le fichier mtl via tiny_obj_loader)
        // si succes, les textures sont bind et has_texture est mis à true
        mesh->material->tryLoadTexture();
        mesh->material->tryLoadSpecularTexture();
        cout << endl;
    }
}
