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

    bool UdpSocket::Send(const Packet32& packet) const
    {
        bool success = false;

        if (packet.Position() > 0 && IsOpen())
        {
            Address32 header = packet.Address();
            sockaddr_in address;
            address.sin_family = AF_INET;
            address.sin_addr.s_addr = htonl(header.Address());
            address.sin_port = htons(header.Port());

            const SOCKET* _socket = reinterpret_cast<const SOCKET*>(_native);
            int sentBytes = sendto(*_socket, (const char*)packet.Buffer(),
                packet.Position(), 0, (sockaddr*)&address, sizeof(sockaddr_in));

            success = sentBytes == packet.Position();
        }

        return success;
    }

    bool UdpSocket::Receive(Packet32& packet) const
    {
        bool success = false;

        if (packet.Capacity() > 0 && IsOpen())
        {
            sockaddr_in from;
            socklen_t length = sizeof(from);

            const SOCKET* _socket = reinterpret_cast<const SOCKET*>(_native);
            packet.Clear();

            int received = recvfrom(*_socket, (char*)packet.Buffer(),
                packet.Capacity(), 0, (sockaddr*)&from, &length);

            if (received > 0)
            {
                packet.Position(received);
                success = true;
            }

            packet.Address(Address32(ntohl(from.sin_addr.s_addr),
                ntohs(from.sin_port)));
        }

        return success;
    }
}
