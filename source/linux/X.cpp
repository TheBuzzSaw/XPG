#include "X.hpp"
#include <cstdio>

namespace XPG
{
    ApplicationMeta* theApplicationMeta = 0;
    std::map< ::Window, WindowMeta* > windows;

    void SetGlxFunctionPointers()
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

    void ProcessEvent(const XEvent& event)
    {
        //printf("processing event\n");
        ::Window window = event.xany.window;
        std::map< ::Window, WindowMeta*>::const_iterator i =
            windows.find(window);

        if (i != windows.end())
        {
            WindowMeta& meta = *i->second;
            Window& window = *meta.object;

            switch (event.type)
            {
                case ButtonPress:
                {
                    MouseState state;
                    state.UserData(meta.events.userData);
                    state.X(event.xmotion.x);
                    state.Y(event.xmotion.y);

                    switch (event.xbutton.button)
                    {
                        case Button1:
                        {
                            if (meta.events.onLeftMouseButtonDown)
                                meta.events.onLeftMouseButtonDown(state);

                            break;
                        }

                        case Button2:
                        {
                            if (meta.events.onMiddleMouseButtonDown)
                                meta.events.onMiddleMouseButtonDown(state);

                            break;
                        }

                        case Button3:
                        {
                            if (meta.events.onRightMouseButtonDown)
                                meta.events.onRightMouseButtonDown(state);

                            break;
                        }

                        case Button4:
                        {
                            if (meta.events.onMouseWheelUp)
                                meta.events.onMouseWheelUp(state);

                            break;
                        }

                        case Button5:
                        {
                            if (meta.events.onMouseWheelDown)
                                meta.events.onMouseWheelDown(state);

                            break;
                        }

                        default: break;
                    }

                    break;
                }

                case ButtonRelease:
                {
                    MouseState state;
                    state.UserData(meta.events.userData);
                    state.X(event.xmotion.x);
                    state.Y(event.xmotion.y);

                    switch (event.xbutton.button)
                    {
                        case Button1:
                        {
                            if (meta.events.onLeftMouseButtonUp)
                                meta.events.onLeftMouseButtonUp(state);

                            break;
                        }

                        case Button2:
                        {
                            if (meta.events.onMiddleMouseButtonUp)
                                meta.events.onMiddleMouseButtonUp(state);

                            break;
                        }

                        case Button3:
                        {
                            if (meta.events.onRightMouseButtonUp)
                                meta.events.onRightMouseButtonUp(state);

                            break;
                        }

                        default: break;
                    }

                    break;
                }

                case MotionNotify:
                {
                    if (meta.events.onMouseMove)
                    {
                        MouseState state;
                        state.UserData(meta.events.userData);
                        state.X(event.xmotion.x);
                        state.Y(event.xmotion.y);
                        meta.events.onMouseMove(state);
                    }

                    break;
                }

                case EnterNotify:
                {
                    if (meta.events.onWindowMouseIn)
                        meta.events.onWindowMouseIn(meta.events.userData);

                    break;
                }

                case LeaveNotify:
                {
                    if (meta.events.onWindowMouseOut)
                        meta.events.onWindowMouseOut(meta.events.userData);

                    break;
                }

                case KeyPress:
                {
                    if (meta.events.onKeyDown)
                    {
                        Key::Code key = LookupKey(event.xkey.keycode);
                        meta.events.onKeyDown(key, meta.events.userData);
                    }

                    break;
                }

                case KeyRelease:
                {
                    if (meta.events.onKeyUp)
                    {
                        Key::Code key = LookupKey(event.xkey.keycode);
                        meta.events.onKeyUp(key, meta.events.userData);
                    }

                    break;
                }

                case Expose:
                {
                    if (meta.events.onWindowExpose)
                        meta.events.onWindowExpose(meta.events.userData);

                    break;
                }

                case ClientMessage:
                {
                    if ((Atom)event.xclient.data.l[0] == meta.wmDeleteMessage)
                    {
                        if (!meta.events.onWindowClose
                            || meta.events.onWindowClose(meta.events.userData))
                        {
                            window.Close();
                        }
                    }

                    break;
                }
            }
        }
    }
}
