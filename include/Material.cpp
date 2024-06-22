#include "Material.h"

void Material::tryLoadTexture()
{
    if(texture != nullptr) delete texture;
    texture = new Texture();
    std::string filepath = getTexturePath();
    if(texture->load(filepath))
    {
        has_texture = true;
        std::cout << "Success Loading Texture: " << filepath << std::endl;
    }
    else
    {
        std::cout << "Failed Loading Texture: " << filepath << std::endl;
    }
}

void Material::tryLoadSpecularTexture()
{
    if(specular_texture != nullptr) delete specular_texture;
    specular_texture = new Texture();
    std::string filepath = getTexturePath();
    if(specular_texture->load(getSpecularTexturePath()))
    {
        has_specular_texture = true;
        std::cout << "Success Loading Specular Texture: " << filepath << std::endl;
    }
    else
    {
        std::cout << "Failed Loading Specular Texture: " << filepath << std::endl;
    }
}

void Material::logInfo()
{
    std::cout << "Material Info" << std::endl;
    std::cout << "  - Texture: " << getTexturePath() << std::endl;
    std::cout << "  - Specular Texture: " << getSpecularTexturePath() << std::endl;
    std::cout << "  - Ambient: " << ambient.x << ", " << ambient.y << ", " << ambient.z << std::endl;
    std::cout << "  - Diffuse: " << diffuse.x << ", " << diffuse.y << ", " << diffuse.z << std::endl;
    std::cout << "  - Specular: " << specular.x << ", " << specular.y << ", " << specular.z << std::endl;
    std::cout << "  - Shininess: " << shininess << std::endl;
}