#include "../include/XPG/Network/TcpSocket.hpp"
#include "Network.hpp"

namespace XPG
{
    struct TcpSocketMeta
    {
        UInt8* buffer;
        SOCKET socket;
    };

    TcpSocket::TcpSocket()
    {
    }

    TcpSocket::~TcpSocket()
    {

    }
}
