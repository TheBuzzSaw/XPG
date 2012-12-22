#include "../include/XPG/OpenGL/VertexArrayObject.hpp"

namespace XPG
{
    VertexArrayObject::VertexArrayObject()
    {
        glGenVertexArrays(1, &_handle);
    }

    VertexArrayObject::~VertexArrayObject()
    {
        glDeleteVertexArrays(1, &_handle);
    }

    void VertexArrayObject::Bind()
    {
        glBindVertexArray(_handle);
    }
}
