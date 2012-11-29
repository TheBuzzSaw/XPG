#include "Windows.hpp"
#include "../../include/XPG/Window.hpp"
#include "../../include/XPG/glew.h"
#include "../../include/XPG/wglew.h"
#include "../../include/XPG/DataTypes.hpp"
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

        Window::MouseEventCallback OnLeftMouseButtonDown;
        Window::MouseEventCallback OnLeftMouseButtonUp;
        Window::MouseEventCallback OnMiddleMouseButtonDown;
        Window::MouseEventCallback OnMiddleMouseButtonUp;
        Window::MouseEventCallback OnRightMouseButtonDown;
        Window::MouseEventCallback OnRightMouseButtonUp;
        Window::MouseEventCallback OnMouseMove;
        Window::MouseExtraButtonEventCallback OnMouseExtraButtonDown;
        Window::MouseExtraButtonEventCallback OnMouseExtraButtonUp;
        Window::MouseWheelEventCallback OnMouseWheel;

        Window::KeyboardEventCallback OnKeyDown;
        Window::KeyboardEventCallback OnKeyUp;

        Window::WindowCloseEventCallback OnWindowClose;

        void* _userData;

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
                    meta->object->MakeCurrent();
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    meta->object->SwapBuffers();
                    break;
                }

                case WM_MOUSEMOVE:
                {
                    if (meta->OnMouseMove != NULL)
                    {
                        MouseState currentState;
                        currentState.UserData(meta->_userData);
                        DetermineMouseState(wparam, lparam, currentState);
                        meta->OnMouseMove(currentState);
                    }
                    break;
                }

                case WM_LBUTTONDOWN:
                {
                    if (meta->OnLeftMouseButtonDown != NULL)
                    {
                        MouseState currentState;
                        currentState.UserData(meta->_userData);
                        DetermineMouseState(wparam, lparam, currentState);
                        meta->OnLeftMouseButtonDown(currentState);
                    }

                    break;
                }

                case WM_LBUTTONUP:
                {
                    if (meta->OnLeftMouseButtonUp != NULL)
                    {
                        MouseState currentState;
                        currentState.UserData(meta->_userData);
                        DetermineMouseState(wparam, lparam, currentState);
                        meta->OnLeftMouseButtonUp(currentState);
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
                    if (meta->OnRightMouseButtonDown != NULL)
                    {
                        MouseState currentState;
                        currentState.UserData(meta->_userData);
                        DetermineMouseState(wparam, lparam, currentState);
                        meta->OnRightMouseButtonDown(currentState);
                    }
                    break;
                }

                case WM_RBUTTONUP:
                {
                    if (meta->OnRightMouseButtonUp != NULL)
                    {
                        MouseState currentState;
                        currentState.UserData(meta->_userData);
                        DetermineMouseState(wparam, lparam, currentState);
                        meta->OnRightMouseButtonUp(currentState);
                    }
                    break;
                }

                case WM_RBUTTONDBLCLK:
                {
                    break;
                }

                case WM_MBUTTONDOWN:
                {
                    if (meta->OnMiddleMouseButtonDown != NULL)
                    {
                        MouseState currentState;
                        currentState.UserData(meta->_userData);
                        DetermineMouseState(wparam, lparam, currentState);
                        meta->OnMiddleMouseButtonDown(currentState);
                    }
                    break;
                }

                case WM_MBUTTONUP:
                {
                    if (meta->OnMiddleMouseButtonUp != NULL)
                    {
                        MouseState currentState;
                        currentState.UserData(meta->_userData);
                        DetermineMouseState(wparam, lparam, currentState);
                        meta->OnMiddleMouseButtonUp(currentState);
                    }
                    break;
                }

                case WM_MBUTTONDBLCLK:
                {

                    break;
                }

                case WM_XBUTTONDOWN:
                {
                    if (meta->OnMouseExtraButtonDown != NULL)
                    {
                        Int32 whichX = GET_XBUTTON_WPARAM(wparam);
                        MouseState currentState;
                        currentState.UserData(meta->_userData);
                        DetermineMouseState(wparam, lparam, currentState);
                        meta->OnMouseExtraButtonDown(currentState, whichX);
                    }

                    break;
                }

                case WM_XBUTTONUP:
                {
                    if (meta->OnMouseExtraButtonUp != NULL)
                    {
                        Int32 whichX = GET_XBUTTON_WPARAM(wparam);
                        MouseState currentState;
                        currentState.UserData(meta->_userData);
                        DetermineMouseState(wparam, lparam, currentState);
                        meta->OnMouseExtraButtonUp(currentState, whichX);
                    }
                    break;
                }

                case WM_XBUTTONDBLCLK:
                {
                    break;
                }


                case WM_MOUSEWHEEL:
                {
                    if (meta->OnMouseWheel != NULL)
                    {
                        Int16 delta = GET_WHEEL_DELTA_WPARAM(wparam);
                        MouseState currentState;
                        currentState.UserData(meta->_userData);
                        DetermineMouseState(wparam, lparam, currentState);

                        char whichWay = delta > 0 ? 1 : -1;
                        meta->OnMouseWheel(currentState, whichWay);
                    }
                    break;
                }

                case WM_KEYDOWN:
                case WM_SYSKEYDOWN:
                {
                    if (meta->OnKeyDown != NULL)
                    {
                        UInt32 key = (lparam & 0x00ff0000) >> 16;
                        bool extended = lparam & (1 << 24);
                        Key::Code keyCode = lookupKey(key, extended);
                        meta->OnKeyDown(keyCode, extended);
                    }
                    break;
                }

                case WM_KEYUP:
                case WM_SYSKEYUP:
                {
                    if (meta->OnKeyUp != NULL)
                    {
                        UInt32 key = (lparam & 0x00ff0000) >> 16;
                        bool extended = lparam & (1 << 24);
                        Key::Code keyCode = lookupKey(key, extended);
                        meta->OnKeyUp(keyCode, extended);
                    }
                    break;
                }

                case WM_CLOSE:
                {
                    bool closeWindow = true;

                    if (meta->OnWindowClose != NULL)
                    {
                        closeWindow = meta->OnWindowClose(meta->_userData);
                    }

                    if (closeWindow)
                        meta->object->Close();
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

    void Window::Draw()
    {
        WindowMeta* meta = (WindowMeta*)_native;
        assert(meta->object != NULL);
        PostMessage(meta->window, WM_PAINT, 0, 0);
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

    void Window::OnLeftMouseButtonDown(MouseEventCallback leftMouseButtonDownCallback)
    {
        WindowMeta* meta = (WindowMeta*)_native;
        meta->OnLeftMouseButtonDown = leftMouseButtonDownCallback;
    }

    void Window::OnLeftMouseButtonUp(MouseEventCallback leftMouseButtonUpCallback)
    {
        WindowMeta* meta = (WindowMeta*)_native;
        meta->OnLeftMouseButtonUp = leftMouseButtonUpCallback;
    }

    void Window::OnMiddleMouseButtonDown(MouseEventCallback middleMouseButtonDownCallback)
    {
        WindowMeta* meta = (WindowMeta*)_native;
        meta->OnMiddleMouseButtonDown = middleMouseButtonDownCallback;
    }

    void Window::OnMiddleMouseButtonUp(MouseEventCallback middleMouseButtonUpCallback)
    {
        WindowMeta* meta = (WindowMeta*)_native;
        meta->OnMiddleMouseButtonUp = middleMouseButtonUpCallback;
    }

    void Window::OnRightMouseButtonDown(MouseEventCallback rightMouseButtonDownCallback)
    {
        WindowMeta* meta = (WindowMeta*)_native;
        meta->OnRightMouseButtonDown = rightMouseButtonDownCallback;
    }

    void Window::OnRightMouseButtonUp(MouseEventCallback rightMouseButtonUpCallback)
    {
        WindowMeta* meta = (WindowMeta*)_native;
        meta->OnRightMouseButtonUp = rightMouseButtonUpCallback;
    }

    void Window::OnMouseMove(MouseEventCallback mouseMoveCallback)
    {
        WindowMeta* meta = (WindowMeta*)_native;
        meta->OnMouseMove = mouseMoveCallback;
    }

    void Window::OnMouseExtraButtonDown(MouseExtraButtonEventCallback mouseExtraButtonDownCallback)
    {
        WindowMeta* meta = (WindowMeta*)_native;
        meta->OnMouseExtraButtonDown = mouseExtraButtonDownCallback;
    }

    void Window::OnMouseExtraButtonUp(MouseExtraButtonEventCallback mouseExtraButtonUpCallback)
    {
        WindowMeta* meta = (WindowMeta*)_native;
        meta->OnMouseExtraButtonUp = mouseExtraButtonUpCallback;
    }

    void Window::OnMouseWheel(MouseWheelEventCallback mouseWheelEventCallback)
    {
        WindowMeta* meta = (WindowMeta*)_native;
        meta->OnMouseWheel = mouseWheelEventCallback;
    }

    void Window::OnKeyDown(KeyboardEventCallback keyDownEventCallback)
    {
        WindowMeta* meta = (WindowMeta*)_native;
        meta->OnKeyDown = keyDownEventCallback;
    }


    void Window::OnKeyUp(KeyboardEventCallback keyUpEventCallback)
    {
        WindowMeta* meta = (WindowMeta*)_native;
        meta->OnKeyUp = keyUpEventCallback;
    }

    void Window::OnWindowClose(WindowCloseEventCallback windowCloseEventCallback)
    {
        WindowMeta* meta = (WindowMeta*)_native;
        meta->OnWindowClose = windowCloseEventCallback;
    }


}
