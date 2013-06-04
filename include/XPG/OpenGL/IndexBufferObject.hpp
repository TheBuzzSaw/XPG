#ifndef XpgIndexBufferObjectHpp
#define XpgIndexBufferObjectHpp

#include "Base.hpp"

namespace XPG
{
    class XpgApi IndexBufferObject
    {
        public:
            IndexBufferObject();
            ~IndexBufferObject();

            void Bind();
            void Load(const GLvoid* block, GLuint bytesPerIndex,
                GLuint indicesPerUnit, GLuint unitCount);
            void DrawElements(GLenum mode = GL_TRIANGLES);

            inline GLuint BytesPerIndex() const { return _bytesPerIndex; }

        private:
            IndexBufferObject(const IndexBufferObject& other);
            IndexBufferObject& operator=(const IndexBufferObject& other);

            GLuint _handle;
            GLenum _indexType;
            GLuint _bytesPerIndex;
            GLenum _usage;
            GLuint _unitCount;
            GLuint _indicesPerUnit;
    };
}

#endif

