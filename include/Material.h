#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include "Vertex.h"
#include "Texture.h"

class Material
{
public:
    // Texture
    std::string asset_path;
    std::string texture_filename;
    std::string specular_texture_filename;

    Texture *texture = nullptr;
    Texture *specular_texture = nullptr;
    
    bool has_texture = false;
    bool has_specular_texture = false;
    // material light
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;

    int ambientLocation = -1, diffuseLocation = -1, specularLocation = -1, shininessLocation = -1;

    Material() : asset_path("./assets/"), texture_filename(""), specular_texture_filename(""),
                 texture(nullptr), specular_texture(nullptr),
                 has_texture(false), has_specular_texture(false),
                 ambient(vec3(1.0f, 1.0f, 1.0f)), diffuse(vec3(1.0f, 1.0f, 1.0f)),
                 specular(vec3(1.0f, 1.0f, 1.0f)), shininess(8.0f),
                 ambientLocation(-1), diffuseLocation(-1), specularLocation(-1), shininessLocation(-1) {}

    ~Material()
    {
        if (texture != nullptr) delete texture;
        if (specular_texture != nullptr) delete specular_texture;
        texture = nullptr;
        specular_texture = nullptr;
    }

    std::string getTexturePath();
    std::string getSpecularTexturePath();

    void setMaterialAttribLocation(int ambientLocation, int diffuseLocation, int specularLocation, int shininessLocation);
    void configUniformMaterialParameters();

    void unbindTexture();

    bool tryLoadTexture();
    bool tryLoadSpecularTexture();

    void logInfo();
};

#endif