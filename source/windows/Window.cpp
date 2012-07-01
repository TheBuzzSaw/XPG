#include "../../include/XPG/Window.hpp"
#include "../../include/XPG/glew.h"
#include "../../include/XPG/wglew.h"
#include "Windows.hpp"
#include <cstring>
#include <cassert>

#include <iostream>
using namespace std;

namespace XPG
{
    struct WindowMeta
    {
        HINSTANCE instance;
        HWND window;
        HDC deviceContext;
        HGLRC renderContext;
        RECT formerPosition;
        Window* object;
    };

    static const char* const ClassName = "XPG";
    static const DWORD BaseStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    static const DWORD BaseExStyle = WS_EX_APPWINDOW;

    static size_t openWindowCount = 0;

    LRESULT CALLBACK WindowCallback(HWND window, UINT message, WPARAM wparam,
        LPARAM lparam)
    {
        WindowMeta* meta = (WindowMeta*)GetWindowLongPtr(window,
            GWL_USERDATA);

        assert(meta != NULL);

        switch (message)
        {
            case WM_CLOSE:
                meta->object->Close();
                break;
        }

        return DefWindowProc(window, message, wparam, lparam);
    }

    LRESULT CALLBACK SetupCallback(HWND window, UINT message, WPARAM wparam,
        LPARAM lparam)
    {
        LRESULT result;

        if (message == WM_NCCREATE)
        {
            LPCREATESTRUCT cs = (LPCREATESTRUCT)lparam;
            SetWindowLongPtr(window, GWL_USERDATA, (long)cs->lpCreateParams);
            SetWindowLongPtr(window, GWL_WNDPROC, (long)WindowCallback);
            result = WindowCallback(window, message, wparam, lparam);
        }
        else
        {
            result = DefWindowProc(window, message, wparam, lparam);
        }

        return result;
    }

    Window::Window()
    {
        cout << "WindowMeta size " << sizeof(WindowMeta) << endl;
        assert(sizeof(WindowMeta) <= sizeof(_native));
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

        if (meta->object)
        {
            return;
        }

        meta->object = this;
        meta->instance = GetModuleHandle(NULL);

        WNDCLASSEX windowClass;
        memset(&windowClass, 0, sizeof(windowClass));
        windowClass.cbSize = sizeof(windowClass);
        windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        windowClass.lpfnWndProc = SetupCallback;
        windowClass.cbClsExtra = 0;
        windowClass.cbWndExtra = sizeof(WindowMeta*);
        windowClass.hInstance = meta->instance;
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

        DWORD exStyle = BaseExStyle | WS_EX_WINDOWEDGE;
        DWORD style = BaseStyle | WS_OVERLAPPEDWINDOW;

        meta->window = CreateWindowEx(exStyle, ClassName, "XPG Reborn",
            style, CW_USEDEFAULT, 0, 640, 480, NULL, NULL, meta->instance,
            meta);

        if (!meta->window)
        {
            cerr << "failed on CreateWindowEx\n";
            return;
        }

        meta->deviceContext = GetDC(meta->window);
        ShowWindow(meta->window, SW_SHOWNORMAL);
        UpdateWindow(meta->window);

        ++openWindowCount;
    }

    void Window::Close()
    {
        WindowMeta* meta = (WindowMeta*)_native;

        if (meta->deviceContext)
        {
            ReleaseDC(meta->window, meta->deviceContext);
        }

        if (meta->window)
        {
            if (!DestroyWindow(meta->window))
            {
                cerr << "error on DestroyWindow\n";
            }

            if (--openWindowCount < 1)
                PostQuitMessage(0);
        }

        memset(_native, 0, sizeof(_native));
    }

    void Window::SwapBuffers()
    {
        WindowMeta* meta = (WindowMeta*)_native;
        ::SwapBuffers(meta->deviceContext);
    }

    void Window::Run()
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
