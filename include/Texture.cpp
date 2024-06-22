#include "Texture.h"

Texture::Texture() : textureID(0) {}

Texture::~Texture()
{
    deleteTexture();
}

bool Texture::load(const std::string &filename)
{
    int texWidth, texHeight, texChannels;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc *texBuffer = stbi_load(filename.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    bool isTexLoaded = texBuffer != nullptr;
    if (!isTexLoaded) return false;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
        // Filtrage trilinéaire en minification et bilineaire en magnification
        // les coordonnees de texture sont limitees a [0 ; 1[
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Si rien n’est specifie pour GL_TEXTURE_WRAP_* c’est GL_REPEAT par defaut
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, texBuffer);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(texBuffer);
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void Texture::bind(GLenum textureUnit, int texPosition, int slot)
{
    glActiveTexture(textureUnit + slot);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // le shader va lire l’unité de texture 0 -> GL_TEXTURE0
    glUniform1i(texPosition, slot);
}

void Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::deleteTexture()
{
    if (textureID > 0)
    {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }
}