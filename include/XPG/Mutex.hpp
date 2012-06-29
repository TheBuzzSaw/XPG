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
            void* _native;
    };
}

#endif
