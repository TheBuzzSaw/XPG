#include "../include/XPG/Network/Packet32.hpp"
#include <cstdlib>
#include <cstring>
#include <cassert>

namespace XPG
{
    Packet32::Packet32(UInt16 capacity)
    {
        assert(capacity > 0);
        _buffer = (UInt8*)malloc(capacity);
        _capacity = _buffer ? capacity : 0;
        _position = 0;
        _failedToWrite = false;

        memset(_buffer, 0, _capacity);
    }

    Packet32::~Packet32()
    {
        free(_buffer);
    }

    void Packet32::Position(UInt16 position)
    {
        if (position < _capacity)
            _position = position;
    }

    void Packet32::Clear()
    {
        _position = 0;
        _failedToWrite = false;
    }

    void Packet32::Write(const void* data, UInt16 size)
    {
        assert(data != 0);
        assert(size > 0);

        if (!_failedToWrite)
        {
            if (FreeSpace() >= size)
            {
                memcpy(_buffer + _position, data, size);
                _position += size;
            }
            else
            {
                _failedToWrite = true;
            }
        }
    }
}
