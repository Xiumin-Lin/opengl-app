#include "Material.h"

using namespace std;

void Material::tryLoadTexture()
{
    if (texture != nullptr) delete texture;
    if (texture_filename.empty()) {
        cout << "No texture file specified" << endl;
        return;
    }
    texture = new Texture();
    string textureFilePath = getTexturePath();
    if (texture->load(textureFilePath))
    {
        has_texture = true;
        cout << "[SUCCESS] Loading Texture: " << textureFilePath << endl;
    }
    else
    {
        cout << "[FAILURE] Loading Texture: " << textureFilePath << endl;
    }
}

void Material::tryLoadSpecularTexture()
{
    if (specular_texture != nullptr) delete specular_texture;
    if (specular_texture_filename.empty()) {
        cout << "No specular texture file specified" << endl;
        return;
    }
    specular_texture = new Texture();
    string specularTextureFilePath = getTexturePath();
    if (specular_texture->load(getSpecularTexturePath()))
    {
        has_specular_texture = true;
        cout << "[SUCCESS] Loading Specular Texture: " << specularTextureFilePath << endl;
    }
    else
    {
        cout << "[FAILURE] Loading Specular Texture: " << specularTextureFilePath << endl;
    }
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