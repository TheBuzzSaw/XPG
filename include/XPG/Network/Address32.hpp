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
            Address32(UInt8 a, UInt8 b, UInt8 c, UInt8 d, UInt16 port);
            Address32(const Address32& other);
            ~Address32();

            Address32& operator=(const Address32& other);

            inline Int32 Address() const
            {
                return *reinterpret_cast<const Int32*>(_address);
            }

            inline UInt16 Port() const
            {
                return _port;
            }

        private:
            UInt8 _address[4];
            UInt16 _port;
    };
}

#endif
