#ifndef XpgUdpSocketHpp
#define XpgUdpSocketHpp

#include "Socket.hpp"
#include "Datagram32.hpp"

namespace XPG
{
    XpgClass UdpSocket : public Socket
    {
        public:
            UdpSocket();
            ~UdpSocket();

            bool Open(UInt16 port);
            bool Send(const Datagram32& packet) const;
            bool Receive(Datagram32& packet) const;

            virtual void Close();
            virtual bool IsOpen() const;
            virtual bool SetBlocking(bool blocking);

        private:
            UdpSocket(const UdpSocket& other);
            UdpSocket& operator=(const UdpSocket& other);

            UInt8 _native[16];
    };
}

#endif
