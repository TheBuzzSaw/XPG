#include "../include/XPG/Network/Address32.hpp"
#include <cstring>

namespace XPG
{
    Address32::Address32()
    {
        memset(_address, 0, sizeof(_address));
        _port = 0;
    }

    Address32::Address32(UInt8 a, UInt8 b, UInt8 c, UInt8 d, UInt16 port)
    {
        _address[0] = a;
        _address[1] = b;
        _address[2] = c;
        _address[3] = d;
        _port = port;
    }

    Address32::Address32(const Address32& other)
    {
        memcpy(_address, other._address, sizeof(_address));
        _port = other._port;
    }

    Address32::~Address32()
    {
    }

    Address32& Address32::operator=(const Address32& other)
    {
        memcpy(_address, other._address, sizeof(_address));
        _port = other._port;
        return *this;
    }
}
