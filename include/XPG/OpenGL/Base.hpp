#ifndef XpgBaseHpp
#define XpgBaseHpp

#include "../Platform.hpp"

#ifdef XpgPlatformAndroid
#   include <EGL/egl.h>
#   include <GLES2/gl2.h>
#   include <GLES2/gl2ext.h>
#else
#   include "glew.h"

#   ifdef XpgPlatformWindows
#       include "wglew.h"
#   else
#       include "glxew.h"
#   endif

#endif

#endif
