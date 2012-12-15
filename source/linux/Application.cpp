#include "../../include/XPG/Application.hpp"
#include "X.hpp"
#include <cstdlib>

namespace XPG
{
    Application::Application()
    {
        if (!theApplicationMeta)
        {
            theApplicationMeta =
                static_cast<ApplicationMeta*>(malloc(sizeof(ApplicationMeta)));

            memset(theApplicationMeta, 0, sizeof(ApplicationMeta));

            theApplicationMeta->display = XOpenDisplay(NULL);
            theApplicationMeta->wmDeleteMessage =
                XInternAtom(theApplicationMeta->display, "WM_DELETE_WINDOW",
                False);
        }

        ++theApplicationMeta->referenceCount;
    }

    Application::~Application()
    {
        if (theApplicationMeta->referenceCount < 2)
        {
            XCloseDisplay(theApplicationMeta->display);
            free(theApplicationMeta);
            theApplicationMeta = 0;
        }
        else
        {
            --theApplicationMeta->referenceCount;
        }
    }

    void Application::Run()
    {
        while (theApplicationMeta->windowCount > 0)
        {
            XEvent event;
            XNextEvent(theApplicationMeta->display, &event);
            ProcessEvent(event);
        }
    }
}
