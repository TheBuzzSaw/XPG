#ifndef XpgUdpSocketHpp
#define XpgUdpSocketHpp

#include "Packet32.hpp"

namespace XPG
{
    XpgClass UdpSocket
    {
        public:
            UdpSocket();
            ~UdpSocket();

            bool Open(UInt16 port);
            void Close();
            bool IsOpen() const;
            bool Send(const Packet32& packet) const;
            bool Receive(Packet32& packet) const;

        private:
            UdpSocket(const UdpSocket& other);
            UdpSocket& operator=(const UdpSocket& other);

            UInt8 _native[16];
    };
}

#endif
