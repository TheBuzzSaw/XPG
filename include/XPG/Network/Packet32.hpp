#ifndef XpgPacket32Hpp
#define XpgPacket32Hpp

#include "Address32.hpp"

namespace XPG
{
    XpgClass Packet32
    {
        public:
            Packet32(UInt16 capacity);
            ~Packet32();

            inline Address32 Address() const
            {
                return _address;
            }

            inline void Address(const Address32& address)
            {
                _address = address;
            }

            inline UInt16 Capacity() const
            {
                return _capacity;
            }

            inline UInt16 ContentLength() const
            {
                return _contentLength;
            }

            inline UInt16 Position() const
            {
                return _position;
            }

            inline UInt16 FreeSpace() const
            {
                return _capacity - _position;
            }

            inline UInt16 ReadableBytesRemaining() const
            {
                return _contentLength - _position;
            }

            inline bool FailedToStream() const
            {
                return _failedToStream;
            }

            inline operator bool() const
            {
                return !_failedToStream;
            }

            inline void Forgive()
            {
                _failedToStream = false;
            }

            inline UInt8* Buffer()
            {
                return _buffer;
            }

            inline const UInt8* Buffer() const
            {
                return _buffer;
            }

            void ContentLength(UInt16 contentLength);
            void Position(UInt16 position);
            void Clear();
            void Write(const void* data, UInt16 size);
            void Read(void* buffer, UInt16 size);

        private:
            Packet32(const Packet32& other);
            Packet32& operator=(const Packet32& other);

            Address32 _address;
            UInt8* _buffer;
            UInt16 _capacity;
            UInt16 _contentLength;
            UInt16 _position;
            bool _failedToStream;
    };

    Packet32& operator<<(Packet32& packet, Int8 data);
    Packet32& operator<<(Packet32& packet, UInt8 data);
    Packet32& operator<<(Packet32& packet, Int16 data);
    Packet32& operator<<(Packet32& packet, UInt16 data);
    Packet32& operator<<(Packet32& packet, Int32 data);
    Packet32& operator<<(Packet32& packet, UInt32 data);
    Packet32& operator<<(Packet32& packet, Int64 data);
    Packet32& operator<<(Packet32& packet, UInt64 data);

    Packet32& operator>>(Packet32& packet, Int8& data);
    Packet32& operator>>(Packet32& packet, UInt8& data);
    Packet32& operator>>(Packet32& packet, Int16& data);
    Packet32& operator>>(Packet32& packet, UInt16& data);
    Packet32& operator>>(Packet32& packet, Int32& data);
    Packet32& operator>>(Packet32& packet, UInt32& data);
    Packet32& operator>>(Packet32& packet, Int64& data);
    Packet32& operator>>(Packet32& packet, UInt64& data);
}

#endif
