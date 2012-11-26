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

        SOCKET attempt = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

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

    bool UdpSocket::Send(const Address32& destination, const void* data,
        int size) const
    {
        bool success = false;

        if (data && size > 0 && IsOpen())
        {
            sockaddr_in address;
            address.sin_family = AF_INET;
            address.sin_addr.s_addr = htonl(destination.Address());
            address.sin_port = htons(destination.Port());

            const SOCKET* _socket = reinterpret_cast<const SOCKET*>(_native);
            int sentBytes = sendto(*_socket, (const char*)data, size, 0,
                (sockaddr*)&address, sizeof(sockaddr_in));

            success = sentBytes == size;
        }

        return success;
    }

    int UdpSocket::Receive(Address32& source, void* data, int size) const
    {
        int received = 0;

        if (data && size > 0 && IsOpen())
        {
            sockaddr_in from;
            socklen_t length = sizeof(from);

            const SOCKET* _socket = reinterpret_cast<const SOCKET*>(_native);

            received = recvfrom(*_socket, (char*)data, size, 0,
                (sockaddr*)&from, &length);

            source = Address32(ntohl(from.sin_addr.s_addr),
                ntohs(from.sin_port));
        }

        return received;
    }
}
