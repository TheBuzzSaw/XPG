#ifndef XpgBaseHpp
#define XpgBaseHpp

#include "../Platform.hpp"

#ifdef XpgPlatformAndroid
#   include <GLES2/gl2.h>
#   include <GLES2/gl2ext.h>
#elif defined (XpgPlatformOsx)
#   include <OpenGL/OpenGL.h>
#   include <OpenGL/gl3.h>
#else
//#   include "glew.h"
#   define GL3_PROTOTYPES 1
#   include "gl3.h"
#endif

namespace XPG
{
    XpgFunction const char* GlErrorText();
}

#endif
