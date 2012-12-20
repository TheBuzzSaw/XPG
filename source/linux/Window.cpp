#include "../../include/XPG/Window.hpp"
#include "X.hpp"
#include <cstring>

namespace XPG
{
    Window::Window()
    {
        memset(_native, 0, sizeof(_native));
        Open();
    }

    Window::~Window()
    {
        Close();
    }

    void Window::Open()
    {
        WindowMeta* meta =
            reinterpret_cast<WindowMeta*>(_native);

        Display* display = theApplicationMeta->display;

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
        glXQueryVersion(display, &glxMajor, &glxMinor);

        int numConfigs = 0;
        GLXFBConfig* fbConfigs = glXChooseFBConfig(display,
            DefaultScreen(display), fbAttributes, &numConfigs);

        XVisualInfo* visualInfo = glXGetVisualFromFBConfig(display,
            fbConfigs[0]);

        long eventMask = ExposureMask | VisibilityChangeMask |
            KeyPressMask | PointerMotionMask | StructureNotifyMask
            | ButtonPressMask | ButtonReleaseMask | FocusChangeMask
            | EnterWindowMask | LeaveWindowMask | KeyReleaseMask;

        XSetWindowAttributes windowAttributes;
        windowAttributes.event_mask = eventMask;
        windowAttributes.border_pixel = 0;
        windowAttributes.bit_gravity = StaticGravity;
        windowAttributes.colormap = XCreateColormap(display,
            RootWindow(display, visualInfo->screen), visualInfo->visual,
            AllocNone);

        GLint windowMask = CWBorderPixel | CWBitGravity | CWEventMask
            | CWColormap;

        meta->window = XCreateWindow(display,
            DefaultRootWindow(display), 20, 20, 640, 480, 0,
            visualInfo->depth, InputOutput, visualInfo->visual, windowMask,
            &windowAttributes);

        meta->wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW",
            False);
        XSetWMProtocols(display, meta->window, &meta->wmDeleteMessage, 1);

        XMapWindow(display, meta->window);

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
            meta->context = glXCreateContextAttribsARB(display,
                fbConfigs[0], 0, True, attribs);
        }
        else
        {
            // No good. Create a legacy context.
            meta->context = glXCreateContext(display, visualInfo, NULL,
                True);
        }

        MakeCurrent(true);

        GLenum e = glewInit();
        (void)e;

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

        glViewport(0, 0, 640, 480);
        glClearColor(theApplicationMeta->windowCount % 2, 0.5f, 0.5f, 1.0f);

        meta->object = this;
        ++theApplicationMeta->windowCount;
        windows[meta->window] = meta;
    }

    void Window::Close()
    {
        WindowMeta* meta = reinterpret_cast<WindowMeta*>(_native);

        if (meta->object)
        {
            Display* display = theApplicationMeta->display;
            glXMakeCurrent(display, None, NULL);
            glXDestroyContext(display, meta->context);
            XUnmapWindow(display, meta->window);
            XDestroyWindow(display, meta->window);
            --theApplicationMeta->windowCount;
            windows.erase(meta->window);

            memset(_native, 0, sizeof(_native));
        }
    }

    void Window::SetTitle(const char* title)
    {
        WindowMeta* meta = reinterpret_cast<WindowMeta*>(_native);

        if (meta->object && title && *title)
        {
            size_t length = strlen(title);
            char* t = new char[length + 1];
            strcpy(t, title);

            XTextProperty titleProperty;
            Status status = XStringListToTextProperty(&t, 1, &titleProperty);

            if (status)
            {
                XSetTextProperty(theApplicationMeta->display, meta->window,
                    &titleProperty, XA_WM_NAME);
                XFree(titleProperty.value);
            }

            delete [] t;
        }
    }

    void Window::MakeCurrent(bool enable)
    {
        Display* display = theApplicationMeta->display;

        if (enable)
        {
            WindowMeta* meta = reinterpret_cast<WindowMeta*>(_native);

            glXMakeCurrent(display, meta->window, meta->context);
        }
        else
        {
            glXMakeCurrent(display, None, NULL);
        }
    }

    void Window::SwapBuffers()
    {
        WindowMeta* meta = reinterpret_cast<WindowMeta*>(_native);

        glXSwapBuffers(theApplicationMeta->display, meta->window);
    }

    const void* Window::UserData() const
    {
        const WindowMeta* meta = reinterpret_cast<const WindowMeta*>(_native);
        return meta->events.userData;
    }

    void Window::UserData(void* userData)
    {
        WindowMeta* meta = reinterpret_cast<WindowMeta*>(_native);
        meta->events.userData = userData;
    }

    void Window::ClearAllEventCallbacks()
    {
        WindowMeta* meta = reinterpret_cast<WindowMeta*>(_native);
        memset(&meta->events, 0, sizeof(EventBatch));
    }

    void Window::OnLeftMouseButtonDown(MouseEventCallback callback)
    {
        WindowMeta* meta = reinterpret_cast<WindowMeta*>(_native);

        meta->events.onLeftMouseButtonDown = callback;
    }

    void Window::OnLeftMouseButtonUp(MouseEventCallback callback)
    {
        WindowMeta* meta = reinterpret_cast<WindowMeta*>(_native);

        meta->events.onLeftMouseButtonUp = callback;
    }

    void Window::OnMiddleMouseButtonDown(MouseEventCallback callback)
    {
        WindowMeta* meta = reinterpret_cast<WindowMeta*>(_native);

        meta->events.onMiddleMouseButtonDown = callback;
    }

    void Window::OnMiddleMouseButtonUp(MouseEventCallback callback)
    {
        WindowMeta* meta = reinterpret_cast<WindowMeta*>(_native);

        meta->events.onMiddleMouseButtonUp = callback;
    }

    void Window::OnRightMouseButtonDown(MouseEventCallback callback)
    {
        WindowMeta* meta = reinterpret_cast<WindowMeta*>(_native);

        meta->events.onRightMouseButtonDown = callback;
    }

    void Window::OnRightMouseButtonUp(MouseEventCallback callback)
    {
        WindowMeta* meta = reinterpret_cast<WindowMeta*>(_native);

        meta->events.onRightMouseButtonUp = callback;
    }

    void Window::OnMouseMove(MouseEventCallback callback)
    {
        WindowMeta* meta = reinterpret_cast<WindowMeta*>(_native);

        meta->events.onMouseMove = callback;
    }

    void Window::OnMouseExtraButtonDown(MouseExtraButtonEventCallback callback)
    {
        WindowMeta* meta = reinterpret_cast<WindowMeta*>(_native);

        meta->events.onMouseExtraButtonDown = callback;
    }

    void Window::OnMouseExtraButtonUp(MouseExtraButtonEventCallback callback)
    {
        WindowMeta* meta = reinterpret_cast<WindowMeta*>(_native);

        meta->events.onMouseExtraButtonUp = callback;
    }

    void Window::OnMouseWheelUp(MouseEventCallback callback)
    {
        WindowMeta* meta = reinterpret_cast<WindowMeta*>(_native);

        meta->events.onMouseWheelUp = callback;
    }

    void Window::OnMouseWheelDown(MouseEventCallback callback)
    {
        WindowMeta* meta = reinterpret_cast<WindowMeta*>(_native);

        meta->events.onMouseWheelDown = callback;
    }

    void Window::OnKeyDown(KeyboardEventCallback callback)
    {
        WindowMeta* meta = reinterpret_cast<WindowMeta*>(_native);

        meta->events.onKeyDown = callback;
    }

    void Window::OnKeyUp(KeyboardEventCallback callback)
    {
        WindowMeta* meta = reinterpret_cast<WindowMeta*>(_native);

        meta->events.onKeyUp = callback;
    }

    void Window::OnFocus(BasicCallback callback)
    {
        WindowMeta* meta = reinterpret_cast<WindowMeta*>(_native);

        meta->events.onWindowFocus = callback;
    }

    void Window::OnBlur(BasicCallback callback)
    {
        WindowMeta* meta = reinterpret_cast<WindowMeta*>(_native);

        meta->events.onWindowBlur = callback;
    }

    void Window::OnMouseIn(BasicCallback callback)
    {
        WindowMeta* meta = reinterpret_cast<WindowMeta*>(_native);

        meta->events.onWindowMouseIn = callback;
    }

    void Window::OnMouseOut(BasicCallback callback)
    {
        WindowMeta* meta = reinterpret_cast<WindowMeta*>(_native);

        meta->events.onWindowMouseOut = callback;
    }

    void Window::OnClose(SuccessCallback callback)
    {
        WindowMeta* meta = reinterpret_cast<WindowMeta*>(_native);

        meta->events.onWindowClose = callback;
    }

    void Window::OnExpose(BasicCallback callback)
    {
        WindowMeta* meta = reinterpret_cast<WindowMeta*>(_native);

        meta->events.onWindowExpose = callback;
    }
}
