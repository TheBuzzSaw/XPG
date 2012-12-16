#ifndef XpgEventBatchHpp
#define XpgEventBatchHpp

#include "../include/XPG/Window.hpp"

namespace XPG
{
    struct EventBatch
    {
        Window::MouseEventCallback onLeftMouseButtonDown;
        Window::MouseEventCallback onLeftMouseButtonUp;
        Window::MouseEventCallback onMiddleMouseButtonDown;
        Window::MouseEventCallback onMiddleMouseButtonUp;
        Window::MouseEventCallback onRightMouseButtonDown;
        Window::MouseEventCallback onRightMouseButtonUp;
        Window::MouseEventCallback onMouseMove;
        Window::MouseEventCallback onMouseWheelUp;
        Window::MouseEventCallback onMouseWheelDown;
        Window::MouseExtraButtonEventCallback onMouseExtraButtonDown;
        Window::MouseExtraButtonEventCallback onMouseExtraButtonUp;

        Window::KeyboardEventCallback onKeyDown;
        Window::KeyboardEventCallback onKeyUp;

        Window::BasicCallback onWindowFocus;
        Window::BasicCallback onWindowBlur;
        Window::BasicCallback onWindowMouseIn;
        Window::BasicCallback onWindowMouseOut;
        Window::BasicCallback onWindowExpose;
        Window::SuccessCallback onWindowClose;

        void* userData;
    };
}

#endif
