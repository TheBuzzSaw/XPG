#ifndef XpgWindowHpp
#define XpgWindowHpp

#include "Platform.hpp"
#include "MouseState.hpp"
#include "../../source/windows/Windows.hpp"


namespace XPG
{
    XpgClass Window
    {
        public:
            Window();
            ~Window();

            typedef void (*MouseEventCallback)(const MouseState&);
            typedef void (*MouseExtraButtonEventCallback)(const MouseState&, int);


            void Open();
            void Close();

            void Draw();
            void SetTitle(const char* title);
            void MakeCurrent();
            void SwapBuffers();

            void OnLeftMouseButtonDown(MouseEventCallback leftMouseButtonDownCallback);
            void OnLeftMouseButtonUp(MouseEventCallback leftMouseButtonUpCallback);
            void OnMiddleMouseButtonDown(MouseEventCallback middleMouseButtonDownCallback);
            void OnMiddleMouseButtonUp(MouseEventCallback middleMouseButtonUpCallback);
            void OnRightMouseButtonDown(MouseEventCallback rightMouseButtonDownCallback);
            void OnRightMouseButtonUp(MouseEventCallback rightMouseButtonUpCallback);
            void OnMouseMove(MouseEventCallback mouseMoveCallback);
            void OnMouseExtraButtonDown(MouseExtraButtonEventCallback mouseExtraButtonDownCallback);
            void OnMouseExtraButtonUp(MouseExtraButtonEventCallback mouseExtraButtonUpCallback);

        private:
            //This must be bigger than the biggest possible size that Window could take up in memory
            //It is used to pull the object out in the WndProc callback
            char _native[1024];

    };
}

#endif
