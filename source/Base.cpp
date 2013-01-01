#include "../include/XPG/OpenGL/Base.hpp"
#define XpgTestError(n) case n: result = #n; break;

#ifndef GL_STACK_UNDERFLOW
#define GL_STACK_UNDERFLOW 0x0504
#endif

#ifndef GL_STACK_OVERFLOW
#define GL_STACK_OVERFLOW 0x0503
#endif

namespace XPG
{
    const char* GlErrorText()
    {
        const char* result;

        switch (glGetError())
        {
            XpgTestError(GL_NO_ERROR);
            XpgTestError(GL_INVALID_ENUM);
            XpgTestError(GL_INVALID_OPERATION);
            XpgTestError(GL_INVALID_FRAMEBUFFER_OPERATION);
            XpgTestError(GL_OUT_OF_MEMORY);
            XpgTestError(GL_STACK_UNDERFLOW);
            XpgTestError(GL_STACK_OVERFLOW);

            default: result = "unknown GL error"; break;
        }

        return result;
    }
}
