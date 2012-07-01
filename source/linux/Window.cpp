#include "../../include/XPG/Window.hpp"
#include "../../include/XPG/glew.h"
#include "../../include/XPG/glxew.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>

#include <iostream>
using namespace std;

namespace XPG
{
    struct WindowMeta
    {
        GLXContext context;
        Display* display;
        ::Window window;
        Atom wmDeleteMessage;
    };

    static void SetGlxFunctionPointers()
    {
        glGenVertexArraysAPPLE = (void(*)(GLsizei, const GLuint*))
            glXGetProcAddressARB((GLubyte*)"glGenVertexArrays");
        glBindVertexArrayAPPLE = (void(*)(const GLuint))
            glXGetProcAddressARB((GLubyte*)"glBindVertexArray");
        glDeleteVertexArraysAPPLE = (void(*)(GLsizei, const GLuint*))
            glXGetProcAddressARB((GLubyte*)"glGenVertexArrays");

        glXCreateContextAttribsARB = (GLXContext(*)(Display*, GLXFBConfig,
            GLXContext, Bool, const int*))glXGetProcAddressARB
            ((GLubyte*)"glXCreateContextAttribsARB");
        glXChooseFBConfig = (GLXFBConfig*(*)(Display*, int, const int*, int*))
            glXGetProcAddressARB((GLubyte*)"glXChooseFBConfig");
        glXGetVisualFromFBConfig = (XVisualInfo*(*)(Display*, GLXFBConfig))
            glXGetProcAddressARB((GLubyte*)"glXGetVisualFromFBConfig");
    }

    Window::Window()
    {
        cout << "WindowMeta size " << sizeof(WindowMeta) << endl;

        memset(_native, 0, sizeof(_native));
        WindowMeta* meta = (WindowMeta*)_native;

        int fbAttributes[] =
            {
                GLX_RENDER_TYPE,   GLX_RGBA_BIT,
                GLX_X_RENDERABLE,  True,
                GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
                GLX_DOUBLEBUFFER,  True,
                GLX_RED_SIZE,      8,
                GLX_BLUE_SIZE,     8,
                GLX_GREEN_SIZE,    8,
                0
            };

        SetGlxFunctionPointers();

        meta->display = XOpenDisplay(NULL);
        int glxMajor = 0;
        int glxMinor = 0;
        glXQueryVersion(meta->display, &glxMajor, &glxMinor);
        cout << "GLX version " << glxMajor << "." << glxMinor << endl;

        int numConfigs = 0;
        GLXFBConfig* fbConfigs = glXChooseFBConfig(meta->display,
            DefaultScreen(meta->display), fbAttributes, &numConfigs);

        XVisualInfo* visualInfo = glXGetVisualFromFBConfig(meta->display,
            fbConfigs[0]);

        long eventMask = ExposureMask | VisibilityChangeMask |
            KeyPressMask | PointerMotionMask | StructureNotifyMask
            | ButtonPressMask | ButtonReleaseMask | FocusChangeMask
            | EnterWindowMask | LeaveWindowMask | KeyReleaseMask;

        XSetWindowAttributes windowAttributes;
        windowAttributes.border_pixel = 0;
        windowAttributes.bit_gravity = StaticGravity;
        windowAttributes.colormap = XCreateColormap(meta->display,
            RootWindow(meta->display, visualInfo->screen), visualInfo->visual,
            AllocNone);

        GLint windowMask = CWBorderPixel | CWBitGravity | CWEventMask
            | CWColormap;

        meta->window = XCreateWindow(meta->display,
            DefaultRootWindow(meta->display))
    }

    Window::~Window()
    {
        WindowMeta* meta = (WindowMeta*)_native;
    }
}
