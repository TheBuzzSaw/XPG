#ifndef XpgTcpSocketHpp
#define XpgTcpSocketHpp

#include "Socket.hpp"
#include "Address32.hpp"
#include "Packet.hpp"

namespace XPG
{
    class TcpListener;

    class XpgApi TcpSocket : public Socket
    {
        public:
            TcpSocket();
            virtual ~TcpSocket();

            Address32 Address() const;
            bool Open(Address32 address);
            bool Open(const TcpListener& listener);
            bool Send(const Packet& packet);
            bool Receive(Packet& packet);

            virtual void Close();
            virtual bool IsOpen() const;
            virtual bool SetBlocking(bool blocking);

        private:
            TcpSocket(const TcpSocket& other);
            TcpSocket& operator=(const TcpSocket& other);

            UInt8 _native[32];
    };
}

#endif
