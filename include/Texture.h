#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <string>

// #define STB_IMAGE_IMPLEMENTATION is in stb_image.cpp
#include "../vendor/stb/stb_image.h"

class Texture {
public:
    GLuint textureID;

    Texture();
    ~Texture();

    bool load(const std::string& filename);
    void bind(GLenum textureUnit, int texPosition, int slot = 0);
    void unbind();
    void deleteTexture();
};

#endif // TEXTURE_H