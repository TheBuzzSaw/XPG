#ifndef XpgDatagram32Hpp
#define XpgDatagram32Hpp

#include "Address32.hpp"
#include "Packet.hpp"

namespace XPG
{
    class XpgApi Datagram32 : public Packet
    {
        public:
            Datagram32(UInt16 capacity);
            virtual ~Datagram32();

            inline Address32 Address() const
            {
                return _address;
            }

            inline void Address(const Address32& address)
            {
                _address = address;
            }

        private:
            Address32 _address;
    };
}

#endif
