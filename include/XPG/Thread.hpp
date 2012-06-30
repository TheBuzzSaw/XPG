#ifndef XpgThreadHpp
#define XpgThreadHpp

#include "Platform.hpp"
#include "DataTypes.hpp"

namespace XPG
{
    XpgClass Thread
    {
        public:
            typedef void (* Entry)(void*);

            Thread();
            ~Thread();

            bool IsRunning() const;
            void Start(Entry entry, void* data = 0);
            void Join();

        private:
            char _native[64];
    };
}

#endif
