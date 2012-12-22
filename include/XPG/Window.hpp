#ifndef XpgWindowHpp
#define XpgWindowHpp

#include "Platform.hpp"
#include "MouseState.hpp"
#include "WindowState.hpp"
#include "Key.hpp"

namespace XPG
{
    XpgClass Window
    {
        public:
            Window();
            ~Window();

            typedef void (*MouseEventCallback)(const MouseState&);
            typedef void (*MouseExtraButtonEventCallback)(const MouseState&,
                Int32);
            typedef void (*BasicCallback)(void*);
            typedef bool (*SuccessCallback)(void*);

            typedef void (*KeyboardEventCallback)(Key::Code, void*);
            typedef void (*WindowEventCallback)(const WindowState&);

            void Open();
            void Close();

            void SetTitle(const char* title);
            void MakeCurrent(bool enable);
            void SwapBuffers();
            void SetVsync(bool enable);

            const void* UserData() const;
            void UserData(void* userData);

            void ClearAllEventCallbacks();

            void OnLeftMouseButtonDown(MouseEventCallback callback);
            void OnLeftMouseButtonUp(MouseEventCallback callback);
            void OnMiddleMouseButtonDown(MouseEventCallback callback);
            void OnMiddleMouseButtonUp(MouseEventCallback callback);
            void OnRightMouseButtonDown(MouseEventCallback callback);
            void OnRightMouseButtonUp(MouseEventCallback callback);
            void OnMouseMove(MouseEventCallback callback);
            void OnMouseExtraButtonDown(MouseExtraButtonEventCallback callback);
            void OnMouseExtraButtonUp(MouseExtraButtonEventCallback callback);
            void OnMouseWheelDown(MouseEventCallback callback);
            void OnMouseWheelUp(MouseEventCallback callback);

            void OnKeyDown(KeyboardEventCallback callback);
            void OnKeyUp(KeyboardEventCallback callback);

            void OnFocus(BasicCallback callback);
            void OnBlur(BasicCallback callback);
            void OnMouseIn(BasicCallback callback);
            void OnMouseOut(BasicCallback callback);
            void OnResize(WindowEventCallback callback);
            void OnClose(SuccessCallback callback);
            void OnExpose(BasicCallback callback);

        private:
            UInt8 _native[2048];
    };
}

#endif
