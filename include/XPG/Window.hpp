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

            typedef void (*MouseEventCallback)(MouseState&);


            void Open();
            void Close();

            void Draw();
            void SetTitle(const char* title);
            void MakeCurrent();
            void SwapBuffers();

            void SetOnLeftMouseButtonDown(MouseEventCallback leftMouseButtonDownCallback);

        private:
            //This must be bigger than the biggest possible size that Window could take up in memory
            //It is used to pull the object out in the WndProc callback
            char _native[1024];

    };
}

#endif
