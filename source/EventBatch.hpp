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
        Window::MouseExtraButtonEventCallback onMouseExtraButtonDown;
        Window::MouseExtraButtonEventCallback onMouseExtraButtonUp;
        Window::MouseWheelEventCallback onMouseWheel;

        Window::KeyboardEventCallback onKeyDown;
        Window::KeyboardEventCallback onKeyUp;

        Window::WindowCloseEventCallback onWindowClose;

        void* userData;
    };
}

#endif
