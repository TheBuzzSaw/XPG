#ifndef XpgTcpSocketHpp
#define XpgTcpSocketHpp

#include "Address32.hpp"
#include "Packet.hpp"

namespace XPG
{
    class TcpListener;

    XpgClass TcpSocket
    {
        public:
            TcpSocket();
            ~TcpSocket();

            Address32 Address() const;
            void Close();
            bool IsOpen() const;
            bool Open(Address32 address);
            bool Open(const TcpListener& listener);
            bool Send(const Packet& packet);
            bool Receive(Packet& packet);
            void SetBlocking(bool blocking);

        private:
            TcpSocket(const TcpSocket& other);
            TcpSocket& operator=(const TcpSocket& other);

            UInt8 _native[32];
    };
}

#endif
