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
                case KeyPress:
                {
                    if (meta.events.onKeyDown)
                    {
                        Key::Code key = LookupKey(event.xkey.keycode);
                        meta.events.onKeyDown(key);
                    }

                    break;
                }

                case KeyRelease:
                {
                    if (meta.events.onKeyUp)
                    {
                        Key::Code key = LookupKey(event.xkey.keycode);
                        meta.events.onKeyUp(key);
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
