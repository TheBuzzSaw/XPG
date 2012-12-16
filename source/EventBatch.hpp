#ifndef XpgEventBatchHpp
#define XpgEventBatchHpp

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

        Window::SuccessCallback onWindowClose;
        Window::BasicCallback onWindowExpose;

        void* userData;
    };
}

#endif
