#ifndef XpgBaseHpp
#define XpgBaseHpp

#include "../Platform.hpp"

#ifdef XpgPlatformAndroid
#   include <GLES2/gl2.h>
#   include <GLES2/gl2ext.h>
#else
#   include "glew.h"
#endif

namespace XPG
{
    XpgFunction const char* GlErrorText();
}

#endif
