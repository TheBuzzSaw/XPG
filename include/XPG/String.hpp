#ifndef XpgStringHpp
#define XpgStringHpp

#include "Platform.hpp"

namespace XPG
{
    XpgClass String
    {
        public:
            String();
            String(const String& other);
            ~String();

            String& operator=(const String& other);

        private:
    };
}

#endif
