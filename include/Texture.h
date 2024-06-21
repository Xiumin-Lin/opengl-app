#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <string>

class Texture {
public:
    Texture();
    ~Texture();

    bool Load(const std::string& filename);
    void Bind(GLenum textureUnit, int texPosition, int spot = 0);
    void Unbind();
    void DeleteTexture();

private:
    GLuint textureID;
};

#endif // TEXTURE_H