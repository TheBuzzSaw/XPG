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

            void Open();
            void Close();

            void Draw();
            void SetTitle(const char* title);
            void MakeCurrent();
            void SwapBuffers();

        private:
            char _native[128];

            MouseState* DetermineMouseState(WPARAM inWParam, LPARAM inLParam);
    };
}

#endif
