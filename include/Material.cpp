#include "Material.h"

using namespace std;

bool Material::tryLoadTexture()
{
    if (texture != nullptr) delete texture;
    if (texture_filename.empty()) {
        cout << "No texture file specified" << endl;
        return false;
    }
    texture = new Texture();
    string textureFilePath = getTexturePath();
    if (texture->load(textureFilePath))
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
    if (specular_texture != nullptr) delete specular_texture;
    if (specular_texture_filename.empty()) {
        cout << "No specular texture file specified" << endl;
        return false;
    }
    specular_texture = new Texture();
    string specularTextureFilePath = getTexturePath();
    if (specular_texture->load(getSpecularTexturePath()))
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
    if (ambientLocation < 0) std::cerr << "Invalid Material AMBIENT attribute location" << std::endl;
    else this->ambientLocation = ambientLocation;
    if (diffuseLocation < 0) std::cerr << "Invalid Material DIFFUSE attribute location" << std::endl;
    else this->diffuseLocation = diffuseLocation;
    if (specularLocation < 0) std::cerr << "Invalid Material SPECULAR attribute location" << std::endl;
    else this->specularLocation = specularLocation;
    if (shininessLocation < 0) std::cerr << "Invalid Material SHININESS attribute location" << std::endl;
    else this->shininessLocation = shininessLocation;
}

void Material::configUniformMaterialParameters() {
    glUniform3f(ambientLocation, ambient.x / 2, ambient.y, ambient.z);
    glUniform3f(diffuseLocation, diffuse.x / 2, diffuse.y, diffuse.z);
    glUniform3f(specularLocation, specular.x, specular.y, specular.z);
    glUniform1f(shininessLocation, shininess);
}

void Material::unbindTexture() {
    if (texture != nullptr) texture->unbind();
    if (specular_texture != nullptr) specular_texture->unbind();
}

void Material::logInfo()
{
    cout << "Material Info" << endl;
    cout << "  - Texture: " << getTexturePath() << endl;
    cout << "  - Specular Texture: " << getSpecularTexturePath() << endl;
    cout << "  - Ambient: " << ambient.x << ", " << ambient.y << ", " << ambient.z << endl;
    cout << "  - Diffuse: " << diffuse.x << ", " << diffuse.y << ", " << diffuse.z << endl;
    cout << "  - Specular: " << specular.x << ", " << specular.y << ", " << specular.z << endl;
    cout << "  - Shininess: " << shininess << endl;
}