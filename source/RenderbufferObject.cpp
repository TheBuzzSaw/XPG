#include "../include/XPG/OpenGL/RenderbufferObject.hpp"

namespace XPG
{
    RenderbufferObject::RenderbufferObject()
    {
        glGenRenderbuffers(1, &_handle);
    }

    RenderbufferObject::~RenderbufferObject()
    {
        glDeleteRenderbuffers(1, &_handle);
    }

    void RenderbufferObject::Bind()
    {
        glBindRenderbuffer(GL_RENDERBUFFER, _handle);
    }
}
