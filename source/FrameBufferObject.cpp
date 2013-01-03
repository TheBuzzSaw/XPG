#include "../include/XPG/OpenGL/FrameBufferObject.hpp"

namespace XPG
{
    FrameBufferObject::FrameBufferObject()
    {
        glGenFramebuffers(1, &_handle);
    }

    FrameBufferObject::~FrameBufferObject()
    {
        glDeleteFramebuffers(1, &_handle);
    }

    void FrameBufferObject::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, _handle);
    }
}
