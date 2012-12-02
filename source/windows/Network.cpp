#include "../Network.hpp"

namespace XPG
{
    bool SetBlockingMode(SOCKET socket, bool blocking)
    {
        bool success = true;

        DWORD nonBlocking = 1;
        if (ioctlsocket(socket, FIONBIO, &nonBlocking) != 0)
        {
            success = false;
        }

        return success;
    }
}
