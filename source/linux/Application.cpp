#include "../../include/XPG/Application.hpp"
#include "../../include/XPG/Window.hpp"
#include "X.hpp"

#include <iostream>
using namespace std;

namespace XPG
{
    Application::Application()
    {
        memset(_native, 0, sizeof(_native));
        ApplicationMeta* meta = (ApplicationMeta*)_native;
        theApplicationMeta = meta;

        meta->display = XOpenDisplay(NULL);
        meta->wmDeleteMessage = XInternAtom(meta->display, "WM_DELETE_WINDOW",
            False);
    }

    Application::~Application()
    {
        ApplicationMeta* meta = (ApplicationMeta*)_native;

        XCloseDisplay(meta->display);
    }

    void Application::Run()
    {
        ApplicationMeta* meta = (ApplicationMeta*)_native;

        while (meta->windowCount > 0)
        {
            XEvent event;
            XNextEvent(meta->display, &event);

            ProcessEvent(event);
        }
    }
}
