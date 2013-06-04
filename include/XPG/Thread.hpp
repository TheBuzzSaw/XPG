#ifndef XpgThreadHpp
#define XpgThreadHpp

#include "Platform.hpp"
#include "TimeSpan.hpp"

namespace XPG
{
    class XpgApi Thread
    {
        public:
            typedef void (*Entry)(void*);

            Thread();
            ~Thread();

            bool IsRunning() const;
            void Start(Entry entry, void* data = 0);
            void Join();
            void Join(TimeSpan timeout);

        private:
            Thread(const Thread& thread)
            {
                // no copying
            }

            Thread& operator=(const Thread& thread)
            {
                // no copying
                return *this;
            }

            char _native[64];
    };
}

#endif
