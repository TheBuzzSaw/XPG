#include "../include/XPG/Network/Packet.hpp"
#include <cstdlib>
#include <cstring>
#include <cassert>

namespace XPG
{
    Packet::Packet(UInt32 capacity)
    {
        assert(capacity > 0);
        _buffer = (UInt8*)malloc(capacity);
        _capacity = _buffer ? capacity : 0;
        _contentLength = 0;
        _position = 0;
        _failedToStream = false;

        memset(_buffer, 0, _capacity);
    }

    Packet::~Packet()
    {
        free(_buffer);
    }

    void Packet::ContentLength(UInt32 contentLength)
    {
        if (contentLength <= _capacity)
        {
            _contentLength = contentLength;

            if (_position > _contentLength)
                _position = _contentLength;
        }
    }

    void Packet::Position(UInt32 position)
    {
        if (position < _contentLength)
            _position = position;
    }

    void Packet::Clear()
    {
        _contentLength = 0;
        _position = 0;
        _failedToStream = false;
    }

    void Packet::Write(const void* data, UInt32 size)
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

    void Packet::Read(void* buffer, UInt32 size)
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

    Packet& operator<<(Packet& packet, const char* data)
    {
        size_t length = data ? strlen(data) : 0;

        if (length > 0)
            packet.Write(data, length);

        return packet;
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

        T* conversion = reinterpret_cast<T*>(result);
        return *conversion;
    }

    template<typename T>
    void WriteInteger(Packet& packet, T data)
    {
#ifdef XpgLittleEndian
        data = EndianFlip<T>(data);
#endif
        packet.Write(&data, sizeof(T));
    }

    template<typename T>
    void ReadInteger(Packet& packet, T& data)
    {
        packet.Read(&data, sizeof(T));

#ifdef XpgLittleEndian
        if (!packet.FailedToStream())
            data = EndianFlip(data);
#endif
    }

    Packet& operator<<(Packet& packet, Int8 data)
    {
        packet.Write(&data, 1);
        return packet;
    }

    Packet& operator<<(Packet& packet, UInt8 data)
    {
        packet.Write(&data, 1);
        return packet;
    }

    Packet& operator<<(Packet& packet, Int16 data)
    {
        WriteInteger(packet, data);
        return packet;
    }

    Packet& operator<<(Packet& packet, UInt16 data)
    {
        WriteInteger(packet, data);
        return packet;
    }

    Packet& operator<<(Packet& packet, Int32 data)
    {
        WriteInteger(packet, data);
        return packet;
    }

    Packet& operator<<(Packet& packet, UInt32 data)
    {
        WriteInteger(packet, data);
        return packet;
    }

    Packet& operator<<(Packet& packet, Int64 data)
    {
        WriteInteger(packet, data);
        return packet;
    }

    Packet& operator<<(Packet& packet, UInt64 data)
    {
        WriteInteger(packet, data);
        return packet;
    }

    Packet& operator>>(Packet& packet, Int8& data)
    {
        packet.Read(&data, 1);
        return packet;
    }

    Packet& operator>>(Packet& packet, UInt8& data)
    {
        packet.Read(&data, 1);
        return packet;
    }

    Packet& operator>>(Packet& packet, Int16& data)
    {
        ReadInteger(packet, data);
        return packet;
    }

    Packet& operator>>(Packet& packet, UInt16& data)
    {
        ReadInteger(packet, data);
        return packet;
    }

    Packet& operator>>(Packet& packet, Int32& data)
    {
        ReadInteger(packet, data);
        return packet;
    }

    Packet& operator>>(Packet& packet, UInt32& data)
    {
        ReadInteger(packet, data);
        return packet;
    }

    Packet& operator>>(Packet& packet, Int64& data)
    {
        ReadInteger(packet, data);
        return packet;
    }

    Packet& operator>>(Packet& packet, UInt64& data)
    {
        ReadInteger(packet, data);
        return packet;
    }
}
