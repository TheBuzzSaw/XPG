#include "../include/XPG/OpenGL/Texture.hpp"

namespace XPG
{
    Texture::Texture(GLenum target)
    {
        _target = target;

        glGenTextures(1, &_handle);
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &_handle);
    }

    void Texture::Bind()
    {
        glBindTexture(_target, _handle);
    }

    void Texture::ProcessParams(const GLenum* params)
    {
        for (const GLenum* i = params; *i; i += 2)
            glTexParameteri(_target, i[0], i[1]);
    }
}
