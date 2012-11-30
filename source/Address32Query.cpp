#include "../include/XPG/Network/Address32Query.hpp"
#include "Network.hpp"

namespace XPG
{
    static bool IsValid(addrinfo* info)
    {
        return info
            && info->ai_family == AF_INET // verify IPv4
            && info->ai_socktype == SOCK_STREAM // verify TCP
            && info->ai_protocol == IPPROTO_TCP // verify TCP again
            ;
    }

    Address32Query::Address32Query(const char* address, const char* port)
    {
        _addresses = 0;
        _count = 0;

        if (address && *address && port && *port)
        {
            addrinfo* result = 0;
            addrinfo hints;

            memset(&hints, 0, sizeof(hints));
            hints.ai_family = AF_INET;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_protocol = IPPROTO_TCP;

            int code = getaddrinfo(address, port, &hints, &result);

            if (code == 0)
            {
                for (addrinfo* i = result; i; i = i->ai_next)
                {
                    if (IsValid(i)) ++_count;
                }

                _addresses = new Address32[_count];

                Address32* address32 = _addresses;

                for (addrinfo* i = result; i; i = i->ai_next)
                {
                    if (IsValid(i))
                    {
                        sockaddr_in* address = (sockaddr_in*)i->ai_addr;

                        *address32++ = Address32(
                            ntohl(address->sin_addr.s_addr),
                            ntohs(address->sin_port)
                            );
                    }
                }

                freeaddrinfo(result);
            }
        }
    }

    Address32Query::~Address32Query()
    {
        delete [] _addresses;
    }

    Address32 Address32Query::GetResult(UInt32 index)
    {
        Address32 result;

        if (index < _count) result = _addresses[index];

        return result;
    }
}
