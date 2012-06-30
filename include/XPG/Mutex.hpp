#ifndef XpgMutexHpp
#define XpgMutexHpp

#include "Platform.hpp"

namespace XPG
{
    XpgClass Mutex
    {
        public:
            Mutex();
            ~Mutex();

            void Lock();
            void Unlock();

        private:
            char _native[64];
    };
}

#endif
