#include "../../include/XPG/Application.hpp"
#include "Windows.hpp"
#include <iostream>

using namespace std;

namespace XPG
{
    Application::Application()
    {
        WNDCLASSEX windowClass;
        memset(&windowClass, 0, sizeof(windowClass));
        windowClass.cbSize = sizeof(windowClass);
        windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        windowClass.lpfnWndProc = SetupCallback;
        windowClass.cbClsExtra = 0;
        windowClass.cbWndExtra = sizeof(void*);
        windowClass.hInstance = GetInstanceModule(NULL);
        windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
        windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        windowClass.hbrBackground = NULL;
        windowClass.lpszMenuName = NULL;
        windowClass.lpszClassName = ClassName;
        windowClass.hIconSm = NULL;

        if (!RegisterClassEx(&windowClass))
        {
            cerr << "failed on RegisterClassEx\n";
        }
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        BOOL result;
        MSG msg;

        while ((result = GetMessage(&msg, NULL, 0, 0)) != 0)
        {
            if (result == -1)
            {
                cerr << "error on GetMessage\n";
                break;
            }
            else
            {
                DispatchMessage(&msg);
            }
        }
    }
}
