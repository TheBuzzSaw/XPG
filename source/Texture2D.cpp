#include "../include/XPG/OpenGL/Texture2D.hpp"

namespace XPG
{
    static const GLenum ShadowMapParams[] = {
        GL_TEXTURE_MIN_FILTER, GL_NEAREST,
        GL_TEXTURE_MAG_FILTER, GL_NEAREST,
        GL_TEXTURE_WRAP_S, GL_CLAMP,
        GL_TEXTURE_WRAP_T, GL_CLAMP,
        0, 0
    };

    Texture2D::Texture2D() : Texture(GL_TEXTURE_2D)
    {
    }

    Texture2D::~Texture2D()
    {
    }

    void Texture2D::LoadShadowMap(GLsizei width, GLsizei height)
    {
        glTexImage2D(Target(), 0, GL_DEPTH_COMPONENT, width, height, 0,
            GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

        ProcessParams(ShadowMapParams);
    }
}
