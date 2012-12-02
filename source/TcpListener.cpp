#include "../include/XPG/Network/TcpListener.hpp"
#include "../include/XPG/Network/Address32.hpp"
#include "TcpListenerMeta.hpp"

namespace XPG
{
    TcpListener::TcpListener()
    {
        memset(_native, 0, sizeof(_native));

        TcpListenerMeta* meta =
            reinterpret_cast<TcpListenerMeta*>(_native);

        meta->backlog = 2;
    }

    TcpListener::~TcpListener()
    {
        Close();
    }

    bool TcpListener::Open(UInt16 port)
    {
        bool success = false;

        if (!IsOpen())
        {
            SOCKET attempt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

            if (attempt != INVALID_SOCKET)
            {
#ifdef XpgPlatformWindows
                BOOL yes = TRUE;
                setsockopt(attempt, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes,
                    sizeof(yes));
#else
                int yes = 1;
                setsockopt(attempt, SOL_SOCKET, SO_REUSEADDR, &yes,
                    sizeof(int));
#endif

                Address32 address(127, 0, 0, 1, port);

                sockaddr_in service;
                service.sin_family = AF_INET;
                service.sin_addr.s_addr = htonl(address.Address());
                service.sin_port = htons(address.Port());

                int result = bind(attempt, (sockaddr*)&service,
                    sizeof(service));

                if (result == SOCKET_ERROR)
                {
                    closesocket(attempt);
                }
                else
                {
                    TcpListenerMeta* meta =
                        reinterpret_cast<TcpListenerMeta*>(_native);

                    meta->socket = attempt;
                    success = true;
                }
            }
        }

        return success;
    }

    bool TcpListener::IsOpen() const
    {
        const TcpListenerMeta* meta =
            reinterpret_cast<const TcpListenerMeta*>(_native);

        return meta->socket > 0;
    }

    void TcpListener::Close()
    {
        TcpListenerMeta* meta =
            reinterpret_cast<TcpListenerMeta*>(_native);

        if (IsOpen())
        {
            closesocket(meta->socket);
            meta->socket = 0;
        }
    }
}
