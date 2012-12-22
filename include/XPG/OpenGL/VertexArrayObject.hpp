#ifndef XpgVertexArrayObjectHpp
#define XpgVertexArrayObjectHpp

#include "Base.hpp"

namespace XPG
{
    XpgClass VertexArrayObject
    {
        public:
            VertexArrayObject();
            ~VertexArrayObject();

            void Bind();

        private:
            VertexArrayObject(const VertexArrayObject& other);
            VertexArrayObject& operator=(const VertexArrayObject& other);

            GLuint _handle;
    };
}

#endif
