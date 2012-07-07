#ifndef XpgXHpp
#define XpgXHpp

#include "../../include/XPG/Window.hpp"
#include "../../include/XPG/glew.h"
#include "../../include/XPG/glxew.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>

namespace XPG
{
    struct WindowMeta
    {
        GLXContext context;
        ::Window window;
        Atom wmDeleteMessage;
        Window* object;
    };

    struct ApplicationMeta
    {
        Display* display;
        Atom wmDeleteMessage;
        size_t windowCount;
    };

    extern ApplicationMeta* theApplicationMeta;
}

#endif
