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

            void Run();
            void SwapBuffers();

        private:
            char _native[128];
    };
}

#endif
