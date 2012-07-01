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

        private:
            char _native[128];
    };
}

#endif
