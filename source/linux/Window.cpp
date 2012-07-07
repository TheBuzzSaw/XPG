#include "../../include/XPG/Window.hpp"
#include "X.hpp"

#include <iostream>
using namespace std;

namespace XPG
{
    struct WindowMeta
    {
        GLXContext context;
        ::Window window;
        Atom wmDeleteMessage;
        Window* object;
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
        Open();
    }

    Window::~Window()
    {
        Close();
    }

    void Window::Open()
    {
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

        int glxMajor = 0;
        int glxMinor = 0;
        glXQueryVersion(GetDisplay(), &glxMajor, &glxMinor);
        cout << "GLX version " << glxMajor << "." << glxMinor << endl;

        int numConfigs = 0;
        GLXFBConfig* fbConfigs = glXChooseFBConfig(GetDisplay(),
            DefaultScreen(GetDisplay()), fbAttributes, &numConfigs);

        XVisualInfo* visualInfo = glXGetVisualFromFBConfig(GetDisplay(),
            fbConfigs[0]);

        long eventMask = ExposureMask | VisibilityChangeMask |
            KeyPressMask | PointerMotionMask | StructureNotifyMask
            | ButtonPressMask | ButtonReleaseMask | FocusChangeMask
            | EnterWindowMask | LeaveWindowMask | KeyReleaseMask;

        XSetWindowAttributes windowAttributes;
        windowAttributes.event_mask = eventMask;
        windowAttributes.border_pixel = 0;
        windowAttributes.bit_gravity = StaticGravity;
        windowAttributes.colormap = XCreateColormap(GetDisplay(),
            RootWindow(GetDisplay(), visualInfo->screen), visualInfo->visual,
            AllocNone);

        GLint windowMask = CWBorderPixel | CWBitGravity | CWEventMask
            | CWColormap;

        meta->window = XCreateWindow(GetDisplay(),
            DefaultRootWindow(GetDisplay()), 20, 20, 640, 480, 0,
            visualInfo->depth, InputOutput, visualInfo->visual, windowMask,
            &windowAttributes);

        meta->wmDeleteMessage = XInternAtom(GetDisplay(), "WM_DELETE_WINDOW",
            False);
        XSetWMProtocols(GetDisplay(), meta->window,
            &meta->wmDeleteMessage, 1);

        XMapWindow(GetDisplay(), meta->window);

        GLint attribs[] =
        {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
            GLX_CONTEXT_MINOR_VERSION_ARB, 2,
            GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
            0
        };

        if (glXCreateContextAttribsARB)
        {
            // Good to go. Create the 3.x context.
            meta->context = glXCreateContextAttribsARB(GetDisplay(),
                fbConfigs[0], 0, True, attribs);
        }
        else
        {
            // No good. Create a legacy context.
            meta->context = glXCreateContext(GetDisplay(), visualInfo, NULL,
                True);
        }

        MakeCurrent();

        GLenum e = glewInit();

        if (!glGenerateMipmap)
            glGenerateMipmap = (void(*)(GLenum))
                glXGetProcAddressARB((GLubyte*)"glGenerateMipmap");

        if (!glGenVertexArrays)
            glGenVertexArrays = (void(*)(GLsizei, GLuint*))
                glGenVertexArraysAPPLE;

        if (!glDeleteVertexArrays)
            glDeleteVertexArrays = glDeleteVertexArraysAPPLE;

        if (!glBindVertexArray)
            glBindVertexArray = glBindVertexArrayAPPLE;

        meta->object = this;
        AcquireWindow();
    }

    void Window::Close()
    {
        WindowMeta* meta = (WindowMeta*)_native;

        if (meta->object)
        {
            glXMakeCurrent(GetDisplay(), None, NULL);
            glXDestroyContext(GetDisplay(), meta->context);
            XDestroyWindow(GetDisplay(), meta->window);
            ReleaseWindow();

            memset(_native, 0, sizeof(_native));
        }
    }

    void Window::Draw()
    {

    }

    void Window::SetTitle(const char* title)
    {
        WindowMeta* meta = (WindowMeta*)_native;

        if (meta->object && title && *title)
        {
            size_t length = strlen(title);
            char* t = new char[length + 1];
            strcpy(t, title);

            XTextProperty titleProperty;
            Status status = XStringListToTextProperty(&t, 1, &titleProperty);

            if (status)
            {
                XSetTextProperty(GetDisplay(), meta->window, &titleProperty,
                    XA_WM_NAME);
                XFree(titleProperty.value);
            }

            delete [] t;
        }
    }

    void Window::MakeCurrent()
    {
        WindowMeta* meta = (WindowMeta*)_native;
        glXMakeCurrent(GetDisplay(), meta->window, meta->context);
    }

    void Window::SwapBuffers()
    {
        WindowMeta* meta = (WindowMeta*)_native;
        glXSwapBuffers(GetDisplay(), meta->window);
    }
}
