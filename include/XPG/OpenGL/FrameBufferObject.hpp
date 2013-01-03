#ifndef XpgFrameBufferObjectHpp
#define XpgFrameBufferObjectHpp

#include "Base.hpp"

namespace XPG
{
    XpgClass FrameBufferObject
    {
        public:
            FrameBufferObject();
            ~FrameBufferObject();

            void Bind();

        private:
            FrameBufferObject(const FrameBufferObject& other);
            FrameBufferObject& operator=(const FrameBufferObject& other);

            GLuint _handle;
    };
}


#endif
