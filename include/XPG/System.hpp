#ifndef XpgSystemHpp
#define XpgSystemHpp

#include "Platform.hpp"

namespace XPG
{
    XpgClass System
    {
        public:
            System();
            virtual ~System();

            virtual void Open() = 0;
            virtual void Close() = 0;
            virtual bool IsOpen() const = 0;
    };
}

#endif
