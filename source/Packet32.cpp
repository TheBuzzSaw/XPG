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
        _contentLength = 0;
        _position = 0;
        _failedToStream = false;

        memset(_buffer, 0, _capacity);
    }

    Packet32::~Packet32()
    {
        free(_buffer);
    }

    void Packet32::ContentLength(UInt16 contentLength)
    {
        if (contentLength <= _capacity)
        {
            _contentLength = contentLength;

            if (_position > _contentLength)
                _position = _contentLength;
        }
    }

    void Packet32::Position(UInt16 position)
    {
        if (position < _contentLength)
            _position = position;
    }

    void Packet32::Clear()
    {
        _contentLength = 0;
        _position = 0;
        _failedToStream = false;
    }

    void Packet32::Write(const void* data, UInt16 size)
    {
        assert(data != 0);
        assert(size > 0);

        if (!_failedToStream)
        {
            if (FreeSpace() >= size)
            {
                memcpy(_buffer + _position, data, size);
                _position += size;

                if (_position > _contentLength) _contentLength = _position;
            }
            else
            {
                _failedToStream = true;
            }
        }
    }

    void Packet32::Read(void* buffer, UInt16 size)
    {
        assert(buffer != 0);
        assert(size > 0);

        if (!_failedToStream)
        {
            if (ReadableBytesRemaining() >= size)
            {
                memcpy(buffer, _buffer + _position, size);
                _position += size;
            }
            else
            {
                _failedToStream = true;
            }
        }
    }

    template<typename T>
    T EndianFlip(T integer)
    {
        char result[sizeof(T)];
        const char* source = (const char*)&integer;
        for (size_t i = 0; i < sizeof(T); ++i)
        {
            size_t j = sizeof(T) - i - 1;
            result[i] = source[j];
        }

        return *((T*)result);
    }

    template<typename T>
    void WriteInteger(Packet32& packet, T data)
    {
#ifdef XpgLittleEndian
        data = EndianFlip(data);
#endif
        packet.Write(&data, sizeof(T));
    }

    template<typename T>
    void ReadInteger(Packet32& packet, T& data)
    {
        packet.Read(&data, sizeof(T));

#ifdef XpgLittleEndian
        if (!packet.FailedToStream())
            data = EndianFlip(data);
#endif
    }

    Packet32& operator<<(Packet32& packet, Int8 data)
    {
        packet.Write(&data, 1);
        return packet;
    }

    Packet32& operator<<(Packet32& packet, UInt8 data)
    {
        packet.Write(&data, 1);
        return packet;
    }

    Packet32& operator<<(Packet32& packet, Int16 data)
    {
        WriteInteger(packet, data);
        return packet;
    }

    Packet32& operator<<(Packet32& packet, UInt16 data)
    {
        WriteInteger(packet, data);
        return packet;
    }

    Packet32& operator<<(Packet32& packet, Int32 data)
    {
        WriteInteger(packet, data);
        return packet;
    }

    Packet32& operator<<(Packet32& packet, UInt32 data)
    {
        WriteInteger(packet, data);
        return packet;
    }

    Packet32& operator<<(Packet32& packet, Int64 data)
    {
        WriteInteger(packet, data);
        return packet;
    }

    Packet32& operator<<(Packet32& packet, UInt64 data)
    {
        WriteInteger(packet, data);
        return packet;
    }

    Packet32& operator>>(Packet32& packet, Int8& data)
    {
        packet.Read(&data, 1);
        return packet;
    }

    Packet32& operator>>(Packet32& packet, UInt8& data)
    {
        packet.Read(&data, 1);
        return packet;
    }

    Packet32& operator>>(Packet32& packet, Int16& data)
    {
        ReadInteger(packet, data);
        return packet;
    }

    Packet32& operator>>(Packet32& packet, UInt16& data)
    {
        ReadInteger(packet, data);
        return packet;
    }

    Packet32& operator>>(Packet32& packet, Int32& data)
    {
        ReadInteger(packet, data);
        return packet;
    }

    Packet32& operator>>(Packet32& packet, UInt32& data)
    {
        ReadInteger(packet, data);
        return packet;
    }

    Packet32& operator>>(Packet32& packet, Int64& data)
    {
        ReadInteger(packet, data);
        return packet;
    }

    Packet32& operator>>(Packet32& packet, UInt64& data)
    {
        ReadInteger(packet, data);
        return packet;
    }
}
