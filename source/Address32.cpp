#include "../include/XPG/Network/Address32.hpp"
#include "Network.hpp"
#include <cstring>

namespace XPG
{
    Address32::Address32()
    {
        _address = 0;
        _port = 0;
    }

    Address32::Address32(UInt16 port)
    {
        _address = (127 << 24) | 1;
        _port = port;
    }

    Address32::Address32(UInt32 address, UInt16 port)
    {
        _address = address;
        _port = port;
    }

    Address32::Address32(UInt8 a, UInt8 b, UInt8 c, UInt8 d, UInt16 port)
    {
        _address = (a << 24) | (b << 16) | (c << 8) | d;
        _port = port;
    }

    Address32::Address32(const Address32& other)
    {
        _address = other._address;
        _port = other._port;
    }

    Address32::~Address32()
    {
    }

    Address32& Address32::operator=(const Address32& other)
    {
        _address = other._address;
        _port = other._port;
        return *this;
    }
}
