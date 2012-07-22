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
            WindowMeta& meta = *i->second;
            Window& window = *meta.object;

            switch (event.type)
            {
                case KeyPress:
                {
                    Event::Details details;
                    details.source = meta.object;
                    Key::Code* key = (Key::Code*)details.data;
                    *key = LookupKey(event.xkey.keycode);
                    window.OnKeyDown().Fire(details);
                    break;
                }

                case Expose:
                {
                    Event::Details details;
                    details.source = meta.object;
                    window.OnExpose().Fire(details);
                    break;
                }

                case ClientMessage:
                    if (event.xclient.data.l[0] == meta.wmDeleteMessage)
                    {
                        Event::Details details;
                        details.source = meta.object;
                        window.OnClose().Fire(details);

                        window.Close();
                    }
                    break;
            }
        }
    }
}
