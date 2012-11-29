#ifndef XpgTcpSocketHpp
#define XpgTcpSocketHpp

#include "Address32.hpp"

namespace XPG
{
    XpgClass TcpSocket
    {
        public:
            TcpSocket();
            ~TcpSocket();

        private:
            TcpSocket(const TcpSocket& other);
            TcpSocket& operator=(const TcpSocket& other);

            UInt8 _native[32];
    };
}

#endif
