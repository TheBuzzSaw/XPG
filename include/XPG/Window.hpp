#ifndef XpgWindowHpp
#define XpgWindowHpp

#include "Platform.hpp"
#include "MouseState.hpp"
#include "Key.hpp"

namespace XPG
{
    XpgClass Window
    {
        public:
            Window();
            ~Window();

            typedef void (*MouseEventCallback)(MouseState&);
            typedef void (*MouseExtraButtonEventCallback)(const MouseState&,
                Int32);
            typedef void (*MouseWheelEventCallback)(MouseState&, Int8);
            typedef bool (*WindowCloseEventCallback)(void*);

            typedef void (*KeyboardEventCallback)(Key::Code);

            void Open();
            void Close();

            void Draw();
            void SetTitle(const char* title);
            void MakeCurrent();
            void SwapBuffers();

            void OnLeftMouseButtonDown(MouseEventCallback callback);
            void OnLeftMouseButtonUp(MouseEventCallback callback);
            void OnMiddleMouseButtonDown(MouseEventCallback callback);
            void OnMiddleMouseButtonUp(MouseEventCallback callback);
            void OnRightMouseButtonDown(MouseEventCallback callback);
            void OnRightMouseButtonUp(MouseEventCallback callback);
            void OnMouseMove(MouseEventCallback callback);
            void OnMouseExtraButtonDown(MouseExtraButtonEventCallback callback);
            void OnMouseExtraButtonUp(MouseExtraButtonEventCallback callback);
            void OnMouseWheel(MouseWheelEventCallback callback);

            void OnKeyDown(KeyboardEventCallback callback);
            void OnKeyUp(KeyboardEventCallback callback);

            void OnWindowClose(WindowCloseEventCallback callback);

        private:
            UInt8 _native[1024];

    };
}

#endif
