#ifndef XpgDatagram32Hpp
#define XpgDatagram32Hpp

#include "Address32.hpp"
#include "Packet.hpp"

namespace XPG
{
    XpgClass Datagram32 : public Packet
    {
        public:
            Datagram32();
            virtual ~Datagram32();

            inline Address32 Source() const
            {
                return _source;
            }

            inline Address32 Destination() const
            {
                return _destination;
            }

            inline void Source(const Address32& address)
            {
                _source = address;
            }

            inline void Destination(const Address32& address)
            {
                _destination = address;
            }

        private:
            Address32 _source;
            Address32 _destination;
    };
}

#endif
