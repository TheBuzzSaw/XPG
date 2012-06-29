#ifndef XpgThreadHpp
#define XpgThreadHpp

#include "DataTypes.hpp"

namespace XPG
{
    class Thread
    {
        public:
            typedef void (* Entry)(void*);

            Thread();
            ~Thread();

            inline bool IsRunning() const { return _isRunning; }

            void Start(Entry entry, void* data = 0);
            void Join();

        private:
            volatile bool _isRunning;
            void* _native;
    };
}

#endif
