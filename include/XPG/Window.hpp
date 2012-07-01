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
            void SwapBuffers();

            static void Run();

        private:
            char _native[128];
    };
}

#endif
