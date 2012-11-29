#ifndef XpgPacketHpp
#define XpgPacketHpp

#include "../Platform.hpp"
#include "../DataTypes.hpp"

namespace XPG
{
    XpgClass Packet
    {
        public:
            Packet(UInt16 capacity);
            virtual ~Packet();

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
            Packet(const Packet& other);
            Packet& operator=(const Packet& other);

            UInt8* _buffer;
            UInt16 _capacity;
            UInt16 _contentLength;
            UInt16 _position;
            bool _failedToStream;
    };

    Packet& operator<<(Packet& packet, const char* data);

    Packet& operator<<(Packet& packet, Int8 data);
    Packet& operator<<(Packet& packet, UInt8 data);
    Packet& operator<<(Packet& packet, Int16 data);
    Packet& operator<<(Packet& packet, UInt16 data);
    Packet& operator<<(Packet& packet, Int32 data);
    Packet& operator<<(Packet& packet, UInt32 data);
    Packet& operator<<(Packet& packet, Int64 data);
    Packet& operator<<(Packet& packet, UInt64 data);

    Packet& operator>>(Packet& packet, Int8& data);
    Packet& operator>>(Packet& packet, UInt8& data);
    Packet& operator>>(Packet& packet, Int16& data);
    Packet& operator>>(Packet& packet, UInt16& data);
    Packet& operator>>(Packet& packet, Int32& data);
    Packet& operator>>(Packet& packet, UInt32& data);
    Packet& operator>>(Packet& packet, Int64& data);
    Packet& operator>>(Packet& packet, UInt64& data);
}

#endif
