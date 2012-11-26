#ifndef XpgAddress32Hpp
#define XpgAddress32Hpp

#include "../Platform.hpp"
#include "../DataTypes.hpp"

namespace XPG
{
    XpgClass Address32
    {
        public:
            Address32();
            explicit Address32(UInt16 port);
            explicit Address32(UInt32 address, UInt16 port);
            explicit Address32(UInt8 a, UInt8 b, UInt8 c, UInt8 d, UInt16 port);
            Address32(const Address32& other);
            ~Address32();

            Address32& operator=(const Address32& other);

            inline UInt32 Address() const { return _address; }
            inline UInt16 Port() const { return _port; }

        private:
            UInt32 _address;
            UInt16 _port;
    };
}

#endif
