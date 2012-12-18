#include "Windows.hpp"
#include "../../include/XPG/Window.hpp"
#include "../../include/XPG/OpenGL/glew.h"
#include "../../include/XPG/OpenGL/wglew.h"
#include "../../include/XPG/DataTypes.hpp"
#include "../EventBatch.hpp"
#include "WindowsKeyMapping.hpp"
#include <cstring>
#include <cassert>

#include <iostream>
using namespace std;

namespace XPG
{
    //This is a cheat to keep the private section of the class out of the header file
    struct WindowMeta
    {
        HWND window;
        HDC deviceContext;
        HGLRC renderContext;
        RECT formerPosition;
        Window* object;
        EventBatch events;
    };

    static const DWORD BaseStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    static const DWORD BaseExStyle = WS_EX_APPWINDOW;

    static size_t openWindowCount = 0;
    static const Window* activeWindow = NULL;

    static void DetermineMouseState(WPARAM wparam, LPARAM lparam, MouseState& state)
    {
        state.X(GET_X_LPARAM(lparam));
        state.Y(GET_Y_LPARAM(lparam));

        if (wparam & 0x0001)
            state.SetButton(MouseState::Left);

        if (wparam & 0x0010)
            state.SetButton(MouseState::Middle);

        if (wparam & 0x0002)
            state.SetButton(MouseState::Right);

        if (wparam & 0x0020)
            state.SetButton(MouseState::X1);

        if (wparam & 0x0040)
            state.SetButton(MouseState::X2);

//        if (wparam & 0x0008)
//        {
//            cerr << "Control key is down" << endl;
//        }
    }

    LRESULT CALLBACK WindowCallback(HWND window, UINT message, WPARAM wparam,
        LPARAM lparam)
    {
        WindowMeta* meta = (WindowMeta*)GetWindowLongPtr(window,
            GWL_USERDATA);

        LRESULT result = 0;

        if (meta && meta->object)
        {
            switch (message)
            {
                case WM_PAINT:
                {
                    if (meta->events.onWindowExpose)
                        meta->events.onWindowExpose(meta->events.userData);
                    meta->object->MakeCurrent();
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    meta->object->SwapBuffers();
                    break;
                }

                case WM_MOUSEMOVE:
                {
                    if (meta->events.onMouseMove)
                    {
                        MouseState currentState;
                        currentState.UserData(meta->events.userData);
                        DetermineMouseState(wparam, lparam, currentState);
                        meta->events.onMouseMove(currentState);
                    }
                    break;
                }

                case WM_LBUTTONDOWN:
                {
                    if (meta->events.onLeftMouseButtonDown)
                    {
                        MouseState currentState;
                        currentState.UserData(meta->events.userData);
                        DetermineMouseState(wparam, lparam, currentState);
                        meta->events.onLeftMouseButtonDown(currentState);
                    }

                    break;
                }

                case WM_LBUTTONUP:
                {
                    if (meta->events.onLeftMouseButtonUp)
                    {
                        MouseState currentState;
                        currentState.UserData(meta->events.userData);
                        DetermineMouseState(wparam, lparam, currentState);
                        meta->events.onLeftMouseButtonUp(currentState);
                    }
                    break;
                }

                case WM_LBUTTONDBLCLK:
                {
                    cerr << "WM_LBUTTONDBLCLK" << endl;
                    break;
                }

                case WM_RBUTTONDOWN:
                {
                    if (meta->events.onRightMouseButtonDown)
                    {
                        MouseState currentState;
                        currentState.UserData(meta->events.userData);
                        DetermineMouseState(wparam, lparam, currentState);
                        meta->events.onRightMouseButtonDown(currentState);
                    }
                    break;
                }

                case WM_RBUTTONUP:
                {
                    if (meta->events.onRightMouseButtonUp)
                    {
                        MouseState currentState;
                        currentState.UserData(meta->events.userData);
                        DetermineMouseState(wparam, lparam, currentState);
                        meta->events.onRightMouseButtonUp(currentState);
                    }
                    break;
                }

                case WM_RBUTTONDBLCLK:
                {
                    break;
                }

                case WM_MBUTTONDOWN:
                {
                    if (meta->events.onMiddleMouseButtonDown)
                    {
                        MouseState currentState;
                        currentState.UserData(meta->events.userData);
                        DetermineMouseState(wparam, lparam, currentState);
                        meta->events.onMiddleMouseButtonDown(currentState);
                    }
                    break;
                }

                case WM_MBUTTONUP:
                {
                    if (meta->events.onMiddleMouseButtonUp)
                    {
                        MouseState currentState;
                        currentState.UserData(meta->events.userData);
                        DetermineMouseState(wparam, lparam, currentState);
                        meta->events.onMiddleMouseButtonUp(currentState);
                    }
                    break;
                }

                case WM_MBUTTONDBLCLK:
                {

                    break;
                }

                case WM_XBUTTONDOWN:
                {
                    if (meta->events.onMouseExtraButtonDown)
                    {
                        Int32 whichX = GET_XBUTTON_WPARAM(wparam);
                        MouseState currentState;
                        currentState.UserData(meta->events.userData);
                        DetermineMouseState(wparam, lparam, currentState);
                        meta->events.onMouseExtraButtonDown(currentState,
                            whichX);
                    }

                    break;
                }

                case WM_XBUTTONUP:
                {
                    if (meta->events.onMouseExtraButtonUp)
                    {
                        Int32 whichX = GET_XBUTTON_WPARAM(wparam);
                        MouseState currentState;
                        currentState.UserData(meta->events.userData);
                        DetermineMouseState(wparam, lparam, currentState);
                        meta->events.onMouseExtraButtonUp(currentState, whichX);
                    }
                    break;
                }

                case WM_XBUTTONDBLCLK:
                {
                    break;
                }


                case WM_MOUSEWHEEL:
                {
                    Int16 delta = GET_WHEEL_DELTA_WPARAM(wparam);
                    MouseState currentState;
                    currentState.UserData(meta->events.userData);
                    DetermineMouseState(wparam, lparam, currentState);

                    if (delta > 0)
                    {
                        if (meta->events.onMouseWheelUp)
                            meta->events.onMouseWheelUp(currentState);
                    }
                    else
                    {
                        if (meta->events.onMouseWheelDown)
                            meta->events.onMouseWheelDown(currentState);
                    }

                    break;
                }

                case WM_KEYDOWN:
                case WM_SYSKEYDOWN:
                {
                    if (meta->events.onKeyDown)
                    {
                        UInt32 key = (lparam & 0x00ff0000) >> 16;
                        bool extended = lparam & (1 << 24);
                        Key::Code keyCode = lookupKey(key, extended);
                        meta->events.onKeyDown(keyCode, meta->events.userData);
                    }
                    break;
                }

                case WM_KEYUP:
                case WM_SYSKEYUP:
                {
                    if (meta->events.onKeyUp)
                    {
                        UInt32 key = (lparam & 0x00ff0000) >> 16;
                        bool extended = lparam & (1 << 24);
                        Key::Code keyCode = lookupKey(key, extended);
                        meta->events.onKeyUp(keyCode, meta->events.userData);
                    }
                    break;
                }

                case WM_CLOSE:
                {
                    if (!meta->events.onWindowClose
                        || meta->events.onWindowClose(meta->events.userData))
                    {
                        meta->object->Close();
                    }

                    break;
                }

                case WM_SETFOCUS:
                {
                    if (meta->events.onWindowFocus)
                        meta->events.onWindowFocus(meta->events.userData);

                    break;
                }

                case WM_KILLFOCUS:
                {
                    if (meta->events.onWindowBlur)
                        meta->events.onWindowBlur(meta->events.userData);

                    break;
                }

                case WM_MOUSELEAVE:
                {
                    if (meta->events.onWindowMouseOut)
                        meta->events.onWindowMouseOut(meta->events.userData);

                    break;
                }

                default:
                {
                    result = DefWindowProc(window, message, wparam, lparam);
                }
            }
        }

        return result;
    }


    /*
    *   This function is setup to be called as WndProc the first time, when a window
    *   is being setup.  It sets up the user data for the window once it is first created
    *   and then reassigns the WndProc callback to WindowCallback
    */
    LRESULT CALLBACK SetupCallback(HWND window, UINT message, WPARAM wparam,
        LPARAM lparam)
    {
        if (message == WM_NCCREATE)
        {
            LPCREATESTRUCT cs = (LPCREATESTRUCT)lparam;

            //setup the userdata for this window
            SetWindowLongPtr(window, GWL_USERDATA, (long)cs->lpCreateParams);

            //reassign the WndProc function to WindowCallback now that we have setup the userdata
            SetWindowLongPtr(window, GWL_WNDPROC, (long)WindowCallback);
        }

        //
        return DefWindowProc(window, message, wparam, lparam);
    }

    void SetupContext(WindowMeta* meta)
    {
        PIXELFORMATDESCRIPTOR pfd;
        memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
        pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL |
            PFD_DRAW_TO_WINDOW;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 32;
        pfd.cDepthBits = 32;
        pfd.iLayerType = PFD_MAIN_PLANE;

        int nPixelFormat = ChoosePixelFormat(meta->deviceContext, &pfd);
        if (nPixelFormat == 0)
        {
            std::cerr << "failed ChoosePixelFormat\n";
            return;
        }

        if (!SetPixelFormat(meta->deviceContext, nPixelFormat, &pfd))
        {
            std::cerr << "failed SetPixelFormat\n";
            return;
        }

        meta->renderContext = wglCreateContext(meta->deviceContext);
        wglMakeCurrent(meta->deviceContext, meta->renderContext);
        activeWindow = meta->object;

        GLenum e = glewInit();

        if (e == GLEW_OK)
        {
            int attributes[] = {
                WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
                WGL_CONTEXT_MINOR_VERSION_ARB, 1,
                0, 0
                };

            if (wglewIsSupported("WGL_ARB_create_context") == 1)
            {
                HGLRC rc = wglCreateContextAttribsARB(meta->deviceContext,
                    NULL, attributes);

                if (rc)
                {
                    wglMakeCurrent(NULL, NULL);
                    wglDeleteContext(meta->renderContext);
                    meta->renderContext = rc;
                    wglMakeCurrent(meta->deviceContext, meta->renderContext);
                }
            }
        }
    }

    Window::Window()
    {
        cout << "WindowMeta size " << sizeof(WindowMeta) << endl;
        assert(sizeof(WindowMeta) <= sizeof(Window::_native));
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

        DWORD exStyle = BaseExStyle | WS_EX_WINDOWEDGE;
        DWORD style = BaseStyle | WS_OVERLAPPEDWINDOW;

        meta->window = CreateWindowEx(exStyle, ClassName, "XPG Reborn",
            style, CW_USEDEFAULT, 0, 640, 480, NULL, NULL,
            GetInstanceModule(NULL), meta);

        if (!meta->window)
        {
            cerr << "failed on CreateWindowEx\n";
            return;
        }

        meta->deviceContext = GetDC(meta->window);

        SetupContext(meta);

        glViewport(0, 0, 640, 480);
        glClearColor(openWindowCount % 2, 0.5f, 0.5f, 1.0f);


        ShowWindow(meta->window, SW_SHOWNORMAL);
        UpdateWindow(meta->window);

        ++openWindowCount;
    }

    void Window::Close()
    {
        WindowMeta* meta = (WindowMeta*)_native;

        if (meta->renderContext)
        {
            wglMakeCurrent(meta->deviceContext, 0);
            wglDeleteContext(meta->renderContext);

            if (activeWindow == this) activeWindow = NULL;
        }

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

            memset(_native, 0, sizeof(_native));

            if (--openWindowCount < 1)
                PostQuitMessage(0);
        }
        else
        {
            memset(_native, 0, sizeof(_native));
        }
    }

    void Window::SetTitle(const char* title)
    {
        WindowMeta* meta = (WindowMeta*)_native;
        if (meta->window != NULL)
            SetWindowText(meta->window, title);
    }

    void Window::MakeCurrent()
    {
        if (activeWindow != this)
        {
            WindowMeta* meta = (WindowMeta*)_native;
            activeWindow = this;
            wglMakeCurrent(meta->deviceContext, meta->renderContext);
        }
    }

    void Window::SwapBuffers()
    {
        WindowMeta* meta = (WindowMeta*)_native;
        ::SwapBuffers(meta->deviceContext);
    }

    void Window::OnLeftMouseButtonDown(MouseEventCallback callback)
    {
        WindowMeta* meta = (WindowMeta*)_native;
        meta->events.onLeftMouseButtonDown = callback;
    }

    void Window::OnLeftMouseButtonUp(MouseEventCallback callback)
    {
        WindowMeta* meta = (WindowMeta*)_native;
        meta->events.onLeftMouseButtonUp = callback;
    }

    void Window::OnMiddleMouseButtonDown(MouseEventCallback callback)
    {
        WindowMeta* meta = (WindowMeta*)_native;
        meta->events.onMiddleMouseButtonDown = callback;
    }

    void Window::OnMiddleMouseButtonUp(MouseEventCallback callback)
    {
        WindowMeta* meta = (WindowMeta*)_native;
        meta->events.onMiddleMouseButtonUp = callback;
    }

    void Window::OnRightMouseButtonDown(MouseEventCallback callback)
    {
        WindowMeta* meta = (WindowMeta*)_native;
        meta->events.onRightMouseButtonDown = callback;
    }

    void Window::OnRightMouseButtonUp(MouseEventCallback callback)
    {
        WindowMeta* meta = (WindowMeta*)_native;
        meta->events.onRightMouseButtonUp = callback;
    }

    void Window::OnMouseMove(MouseEventCallback callback)
    {
        WindowMeta* meta = (WindowMeta*)_native;
        meta->events.onMouseMove = callback;
    }

    void Window::OnMouseExtraButtonDown(MouseExtraButtonEventCallback callback)
    {
        WindowMeta* meta = (WindowMeta*)_native;
        meta->events.onMouseExtraButtonDown = callback;
    }

    void Window::OnMouseExtraButtonUp(MouseExtraButtonEventCallback callback)
    {
        WindowMeta* meta = (WindowMeta*)_native;
        meta->events.onMouseExtraButtonUp = callback;
    }

    void Window::OnMouseWheelDown(MouseEventCallback callback)
    {
        WindowMeta* meta = (WindowMeta*)_native;
        meta->events.onMouseWheelDown = callback;
    }

    void Window::OnMouseWheelUp(MouseEventCallback callback)
    {
        WindowMeta* meta = (WindowMeta*)_native;
        meta->events.onMouseWheelUp = callback;
    }

    void Window::OnKeyDown(KeyboardEventCallback callback)
    {
        WindowMeta* meta = (WindowMeta*)_native;
        meta->events.onKeyDown = callback;
    }

    void Window::OnKeyUp(KeyboardEventCallback callback)
    {
        WindowMeta* meta = (WindowMeta*)_native;
        meta->events.onKeyUp = callback;
    }

    void Window::OnFocus(BasicCallback callback)
    {
        WindowMeta* meta = (WindowMeta*)_native;
        meta->events.onWindowFocus = callback;
    }

    void Window::OnBlur(BasicCallback callback)
    {
        WindowMeta* meta = (WindowMeta*)_native;
        meta->events.onWindowBlur = callback;
    }

    void Window::OnMouseIn(BasicCallback callback)
    {
        WindowMeta* meta = (WindowMeta*)_native;
        meta->events.onWindowMouseIn = callback;
    }

    void Window::OnMouseOut(BasicCallback callback)
    {
        WindowMeta* meta = (WindowMeta*)_native;
        meta->events.onWindowMouseOut = callback;
    }

    void Window::OnExpose(BasicCallback callback)
    {
        WindowMeta* meta = (WindowMeta*)_native;
        meta->events.onWindowExpose = callback;
    }

    void Window::OnClose(SuccessCallback callback)
    {
        WindowMeta* meta = (WindowMeta*)_native;
        meta->events.onWindowClose = callback;
    }

    void Window::UserData(void* userData)
    {
        WindowMeta* meta = (WindowMeta*)_native;
        meta->events.userData = userData;
    }

    void Window::ClearAllEventCallbacks()
    {
        WindowMeta* meta = (WindowMeta*)_native;
        memset(&meta->events, 0, sizeof(EventBatch));
    }
}
