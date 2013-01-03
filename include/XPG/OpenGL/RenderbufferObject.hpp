#ifndef XpgRenderbufferObjectHpp
#define XpgRenderbufferObjectHpp

#include "Base.hpp"

namespace XPG
{
    XpgClass RenderbufferObject
    {
        public:
            RenderbufferObject();
            ~RenderbufferObject();

            void Bind();

        private:
            RenderbufferObject(const RenderbufferObject& other);
            RenderbufferObject& operator=(const RenderbufferObject& other);

            GLuint _handle;
    };
}

#endif

