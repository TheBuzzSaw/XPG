#include "../include/XPG/OpenGL/IndexBufferObject.hpp"
#include <cassert>

namespace XPG
{
    IndexBufferObject::IndexBufferObject()
    {
        _handle = 0;
        _indexType = 0;
        _bytesPerIndex = 0;
        _unitCount = 0;
        _indicesPerUnit = 0;
        _usage = GL_STATIC_DRAW;

        glGenBuffers(1, &_handle);
    }

    IndexBufferObject::~IndexBufferObject()
    {
        glDeleteBuffers(1, &_handle);
    }

    void IndexBufferObject::Load(const GLvoid* block, GLuint bytesPerIndex,
        GLuint indicesPerUnit, GLuint unitCount)
    {
        assert(block != 0);
        assert(bytesPerIndex > 0);
        assert(indicesPerUnit > 0);
        assert(unitCount > 0);

        switch (bytesPerIndex)
        {
            case 1:
            {
                _indexType = GL_UNSIGNED_BYTE;
                _bytesPerIndex = 1;
                break;
            }

            case 2:
            {
                _indexType = GL_UNSIGNED_SHORT;
                _bytesPerIndex = 2;
                break;
            }

            case 4:
            {
                _indexType = GL_UNSIGNED_INT;
                _bytesPerIndex = 4;
                break;
            }

            default:
            {
                _indexType = 0;
                _bytesPerIndex = 0;
                break;
            }
        }

        assert(_indexType != 0);

        _indicesPerUnit = indicesPerUnit;
        _unitCount = unitCount;
        GLuint totalBytes = _unitCount * _indicesPerUnit * _bytesPerIndex;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _handle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, totalBytes, block, _usage);
    }

    void IndexBufferObject::DrawElements(GLenum mode)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _handle);
        glDrawElements(mode, _unitCount * _indicesPerUnit, _indexType, 0);
    }
}
