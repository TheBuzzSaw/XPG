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
            Mutex(const Mutex& mutex)
            {
                // no copying
            }

            Mutex& operator=(const Mutex& mutex)
            {
                // no copying
                return *this;
            }

            char _native[64];
    };
}

#endif
