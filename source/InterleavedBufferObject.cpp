#include "../include/XPG/OpenGL/InterleavedBufferObject.hpp"
#include <cstring>

namespace XPG
{
    static inline const GLvoid* BufferOffset(GLuint offset)
    {
        return static_cast<char*>(0) + offset;
    }

    InterleavedBufferObject::InterleavedBufferObject()
    {
        _handle = 0;
        _bytesPerVertex = 0;
        _vertexCount = 0;
        _mappingCount = 0;
        memset(_mappings, 0, sizeof(_mappings));

        glGenBuffers(1, &_handle);
    }

    InterleavedBufferObject::~InterleavedBufferObject()
    {
        glDeleteBuffers(1, &_handle);
    }

    void InterleavedBufferObject::Add(GLuint index, GLuint count)
    {
        FieldMapping& fm = _mappings[_mappingCount++];
        fm.index = index;
        fm.count = count;

        _bytesPerVertex += count * sizeof(GLfloat);
    }

    void InterleavedBufferObject::Load(const GLfloat* block, GLuint count)
    {
        _vertexCount = count;
        GLuint totalBytes = _vertexCount * _bytesPerVertex;

        glBindBuffer(GL_ARRAY_BUFFER, _handle);
        glBufferData(GL_ARRAY_BUFFER, totalBytes, block, GL_STATIC_DRAW);
    }

    void InterleavedBufferObject::Draw(IndexBufferObject& ibo, GLenum mode)
    {
        EnableArrays();
        ibo.DrawElements(mode);
        DisableArrays();
    }

    void InterleavedBufferObject::EnableArrays()
    {
        glBindBuffer(GL_ARRAY_BUFFER, _handle);
        GLuint offset = 0;

        for (GLuint i = 0; i < _mappingCount; ++i)
        {
            FieldMapping& fm = _mappings[i];
            glVertexAttribPointer(fm.index, fm.count, GL_FLOAT, GL_FALSE,
                _bytesPerVertex, BufferOffset(offset));
            glEnableVertexAttribArray(fm.index);

            offset += fm.count * sizeof(GLfloat);
        }
    }

    void InterleavedBufferObject::DisableArrays()
    {
        for (GLuint i = 0; i < _mappingCount; ++i)
        {
            FieldMapping& fm = _mappings[i];
            glDisableVertexAttribArray(fm.index);
        }
    }
}
