#ifndef XpgTcpListenerMetaHpp
#define XpgTcpListenerMetaHpp

#include "Network.hpp"

namespace XPG
{
    struct TcpListenerMeta
    {
        SOCKET socket;
        int backlog;
    };
}

#endif
