#ifndef XpgInterleavedBufferObjectHpp
#define XpgInterleavedBufferObjectHpp

#include "IndexBufferObject.hpp"

namespace XPG
{
    XpgClass InterleavedBufferObject
    {
        public:
            InterleavedBufferObject();
            ~InterleavedBufferObject();

            void Add(GLuint index, GLuint count);
            void Load(const GLfloat* block, GLuint count);
            void Draw(IndexBufferObject& ibo, GLenum mode = GL_TRIANGLES);

        private:
            InterleavedBufferObject(const InterleavedBufferObject& other);
            InterleavedBufferObject& operator=(const InterleavedBufferObject&
                other);

            void EnableArrays();
            void DisableArrays();

            struct FieldMapping
            {
                GLuint index;
                GLuint count;
            };

            static const GLuint MaxMappings = 16;

            GLuint _handle;
            GLuint _bytesPerVertex;
            GLuint _vertexCount;
            FieldMapping _mappings[MaxMappings];
            GLuint _mappingCount;
    };
}

#endif

