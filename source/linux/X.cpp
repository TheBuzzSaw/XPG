#include "X.hpp"

namespace XPG
{
    ApplicationMeta* theApplicationMeta = 0;
    std::map< ::Window, WindowMeta*> windows;

    void ProcessEvent(const XEvent& event)
    {
        ::Window window = event.xany.window;
        std::map< ::Window, WindowMeta*>::const_iterator i =
            windows.find(window);

        if (i != windows.end())
        {
            WindowMeta* meta = i->second;

            switch (event.type)
            {
                case Expose:
                    meta->object->MakeCurrent();
                    glClear(GL_COLOR_BUFFER_BIT);
                    meta->object->SwapBuffers();
                    break;

                case ClientMessage:
                    if (event.xclient.data.l[0] == meta->wmDeleteMessage)
                    {
                        meta->object->Close();
                    }
                    break;
            }
        }
    }
}
