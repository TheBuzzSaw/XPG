#include "../../include/XPG/Application.hpp"
#include "../../include/XPG/Window.hpp"
#include "X.hpp"

#include <iostream>
using namespace std;

namespace XPG
{
    Application::Application()
    {
        SetupX11();
    }

    Application::~Application()
    {
        CloseX11();
    }

    void Application::Run()
    {
        while (GetWindowCount() > 0)
        {
            XEvent event;
            XNextEvent(GetDisplay(), &event);

            cerr << GetWindowCount() << endl;

            //ReleaseWindow();
        }
    }
}
