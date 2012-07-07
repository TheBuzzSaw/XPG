#ifndef XpgXHpp
#define XpgXHpp

#include "../../include/XPG/glew.h"
#include "../../include/XPG/glxew.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>

namespace XPG
{
    void SetupX11();
    void CloseX11();
    Display* GetDisplay();

    void AcquireWindow();
    void ReleaseWindow();
    size_t GetWindowCount();
}

#endif
