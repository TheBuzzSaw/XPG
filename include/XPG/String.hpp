#ifndef XpgStringHpp
#define XpgStringHpp

#include "Platform.hpp"

namespace XPG
{
    class XpgApi String
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
