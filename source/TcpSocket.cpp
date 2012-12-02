#include "../include/XPG/Network/TcpSocket.hpp"
#include "../include/XPG/Network/TcpListener.hpp"
#include "TcpListenerMeta.hpp"
#include <cstdlib>

namespace XPG
{
    struct TcpSocketMeta
    {
        SOCKET socket;
        Address32 address;
    };

    TcpSocket::TcpSocket()
    {
        memset(_native, 0, sizeof(_native));
    }

    TcpSocket::~TcpSocket()
    {
        Close();
    }

    void TcpSocket::Close()
    {
        TcpSocketMeta* meta =
            reinterpret_cast<TcpSocketMeta*>(_native);

        if (IsOpen())
        {
            closesocket(meta->socket);
            meta->socket = 0;
            meta->address = Address32();
        }
    }

    bool TcpSocket::IsOpen() const
    {
        const TcpSocketMeta* meta =
            reinterpret_cast<const TcpSocketMeta*>(_native);

        return meta->socket > 0;
    }

    bool TcpSocket::Open(Address32 address)
    {
        bool success = false;

        TcpSocketMeta* meta =
            reinterpret_cast<TcpSocketMeta*>(_native);

        if (address && !IsOpen())
        {
            sockaddr_in sa;
            sa.sin_family = AF_INET;
            sa.sin_addr.s_addr = htonl(address.Address());
            sa.sin_port = htons(address.Port());

            SOCKET attempt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

            if (attempt > 0)
            {
                int result = connect(attempt, (const sockaddr*)&sa, sizeof(sa));

                if (result == SOCKET_ERROR)
                {
                    closesocket(attempt);
                }
                else
                {
                    meta->socket = attempt;
                    success = true;
                }
            }
        }

        return success;
    }

    bool TcpSocket::Open(const TcpListener& listener)
    {
        bool success = false;

        if (!IsOpen() && listener.IsOpen())
        {
            const TcpListenerMeta* tlm =
                reinterpret_cast<const TcpListenerMeta*>(listener._native);

            if (listen(tlm->socket, tlm->backlog) == 0)
            {
                sockaddr_in from;
                socklen_t fromlen = sizeof(from);

                TcpSocketMeta* meta =
                    reinterpret_cast<TcpSocketMeta*>(_native);

                meta->socket = accept(tlm->socket, (sockaddr*)&from, &fromlen);
                meta->address = Address32(ntohl(from.sin_addr.s_addr),
                    ntohs(from.sin_port));

                success = true;
            }
        }

        return success;
    }

    bool TcpSocket::Send(const Packet& packet)
    {
        bool success = false;

        if (IsOpen())
        {
            TcpSocketMeta* meta =
                reinterpret_cast<TcpSocketMeta*>(_native);

            UInt16 remaining = packet.ContentLength();
            UInt16 sent = 0;

            success = true;

            while (remaining > 0)
            {
                int result = send(meta->socket,
                    (const char*)packet.Buffer() + sent, remaining, 0);

                if (result != SOCKET_ERROR && result > 0)
                {
                    sent += result;
                    remaining -= result;
                }
                else
                {
                    Close();
                    success = false;
                    break;
                }
            }
        }

        return success;
    }

    bool TcpSocket::Receive(Packet& packet)
    {
        bool success = false;
        packet.Clear();

        if (IsOpen())
        {
            TcpSocketMeta* meta =
                reinterpret_cast<TcpSocketMeta*>(_native);

            int result = recv(meta->socket, (char*)packet.Buffer(),
                packet.Capacity(), 0);

            if (result > 0)
            {
                success = true;
                packet.ContentLength(result);
            }
        }

        return success;
    }

    bool TcpSocket::SetBlocking(bool blocking)
    {
        bool success = false;

        if (IsOpen())
        {
            TcpSocketMeta* meta =
                reinterpret_cast<TcpSocketMeta*>(_native);

            success = SetBlockingMode(meta->socket, blocking);
        }

        return success;
    }
}
