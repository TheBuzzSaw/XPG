#include "../Network.hpp"

namespace XPG
{
    bool SetBlockingMode(SOCKET socket, bool blocking)
    {
        bool success = true;

        int nonBlocking = 1;
        if (fcntl(socket, F_SETFL, O_NONBLOCK, nonBlocking) == -1)
        {
            success = false;
        }

        return success;
    }
}
