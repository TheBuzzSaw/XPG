#include "../include/XPG/Network/UdpSocket.hpp"
#include "Network.hpp"

namespace XPG
{
    UdpSocket::UdpSocket()
    {
    }

    UdpSocket::~UdpSocket()
    {
        Close();
    }

    bool UdpSocket::Open(UInt16 port)
    {
        bool success = false;

        SOCKET attempt = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

        if (attempt > 0)
        {
            sockaddr_in address;
            address.sin_family = AF_INET;
            address.sin_addr.s_addr = INADDR_ANY;
            address.sin_port = htons(port);

            if (bind(attempt, (const sockaddr*)&address, sizeof(sockaddr_in))
                >= 0)
            {
                SOCKET* _socket = reinterpret_cast<SOCKET*>(_native);
                *_socket = attempt;
                success = true;
            }
        }

        return success;
    }

    void UdpSocket::Close()
    {
        if (IsOpen())
        {
            SOCKET* _socket = reinterpret_cast<SOCKET*>(_native);
            closesocket(*_socket);
            *_socket = 0;
        }
    }

    bool UdpSocket::IsOpen() const
    {
        const SOCKET* _socket = reinterpret_cast<const SOCKET*>(_native);
        return *_socket > 0;
    }
}
