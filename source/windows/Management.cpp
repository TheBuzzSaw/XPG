#include "../../include/XPG/Network/Management.hpp"
#include "../Network.hpp"

namespace XPG
{
    bool OpenSockets()
    {
        WSADATA wsaData;
        return WSAStartup(MAKEWORD(2, 2), &wsaData) == NO_ERROR;
    }

    void CloseSockets()
    {
        WSACleanup();
    }
}
