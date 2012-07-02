#ifndef XpgWindowHpp
#define XpgWindowHpp

#include "Platform.hpp"

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
    };
}

#endif
