#include "Material.h"

using namespace std;

bool Material::tryLoadTexture()
{
    if (texture != nullptr)
        delete texture;
    if (texture_filename.empty())
    {
        cout << "No texture file specified" << endl;
        return false;
    }
    texture = new Texture();
    string textureFilePath = getTexturePath();
    if (texture->load(textureFilePath)) // load & bind diffuse texture with default texture config
    {
        has_texture = true;
        cout << "[SUCCESS] Loading Texture: " << textureFilePath << endl;
        return true;
    }
    cout << "[FAILURE] Loading Texture: " << textureFilePath << endl;
    return false;
}

bool Material::tryLoadSpecularTexture()
{
    if (specular_texture != nullptr)
        delete specular_texture;
    if (specular_texture_filename.empty())
    {
        cout << "No specular texture file specified" << endl;
        return false;
    }
    specular_texture = new Texture();
    string specularTextureFilePath = getTexturePath();
    if (specular_texture->load(getSpecularTexturePath())) // load & bind specular texture with default texture config
    {
        has_specular_texture = true;
        cout << "[SUCCESS] Loading Specular Texture: " << specularTextureFilePath << endl;
        return true;
    }
    cout << "[FAILURE] Loading Specular Texture: " << specularTextureFilePath << endl;
    return false;
}

void Material::setMaterialAttribLocation(int ambientLocation, int diffuseLocation, int specularLocation, int shininessLocation)
{
    if (ambientLocation < 0)
        std::cerr << "Invalid Material AMBIENT attribute location" << std::endl;
    else
        this->ambientLocation = ambientLocation;
    if (diffuseLocation < 0)
        std::cerr << "Invalid Material DIFFUSE attribute location" << std::endl;
    else
        this->diffuseLocation = diffuseLocation;
    if (specularLocation < 0)
        std::cerr << "Invalid Material SPECULAR attribute location" << std::endl;
    else
        this->specularLocation = specularLocation;
    if (shininessLocation < 0)
        std::cerr << "Invalid Material SHININESS attribute location" << std::endl;
    else
        this->shininessLocation = shininessLocation;
}

void Material::configUniformMaterialParameters()
{
    glUniform3f(ambientLocation, ambient.x, ambient.y, ambient.z);
    glUniform3f(diffuseLocation, diffuse.x, diffuse.y, diffuse.z);
    glUniform3f(specularLocation, specular.x, specular.y, specular.z);
    glUniform1f(shininessLocation, shininess);
}

void Material::unbindTexture()
{
    if (texture != nullptr)
        texture->unbind();
    if (specular_texture != nullptr)
        specular_texture->unbind();
}

std::string Material::getTexturePath()
{
    if (texture_filename.empty())
        return "";
    return asset_path + texture_filename;
}

std::string Material::getSpecularTexturePath()
{
    if (specular_texture_filename.empty())
        return "";
    return asset_path + specular_texture_filename;
}

void Material::logInfo()
{
    cout << "Material Info" << endl;
    string texturePath = getTexturePath();
    if (texturePath.empty())
        cout << "  - Texture: None" << endl;
    else
        cout << "  - Texture: " << texturePath << endl;
    string specularTexturePath = getSpecularTexturePath();
    if (specularTexturePath.empty())
        cout << "  - Specular Texture: None" << endl;
    else
        cout << "  - Specular Texture: " << specularTexturePath << endl;
    cout << "  - Ambient: " << ambient.x << ", " << ambient.y << ", " << ambient.z << endl;
    cout << "  - Diffuse: " << diffuse.x << ", " << diffuse.y << ", " << diffuse.z << endl;
    cout << "  - Specular: " << specular.x << ", " << specular.y << ", " << specular.z << endl;
    cout << "  - Shininess: " << shininess << endl;
}