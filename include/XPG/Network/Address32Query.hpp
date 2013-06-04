#ifndef XpgAddress32QueryHpp
#define XpgAddress32QueryHpp

#include "Address32.hpp"

namespace XPG
{
    class XpgApi Address32Query
    {
        public:
            Address32Query(const char* address, const char* port);
            ~Address32Query();

            inline UInt16 Count() const
            {
                return _count;
            }

            Address32 GetResult(UInt32 index);

        private:
            Address32Query(const Address32Query& other);
            Address32Query& operator=(const Address32Query& other);

            Address32* _addresses;
            UInt32 _count;
    };
}

#endif
