#ifndef XpgPacketHpp
#define XpgPacketHpp

#include "../Platform.hpp"
#include "../DataTypes.hpp"

namespace XPG
{
    XpgClass Packet
    {
        public:
            Packet(UInt32 capacity);
            virtual ~Packet();

            inline UInt32 Capacity() const
            {
                return _capacity;
            }

            inline UInt32 ContentLength() const
            {
                return _contentLength;
            }

            inline UInt32 Position() const
            {
                return _position;
            }

            inline UInt32 FreeSpace() const
            {
                return _capacity - _position;
            }

            inline UInt32 ReadableBytesRemaining() const
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

            void ContentLength(UInt32 contentLength);
            void Position(UInt32 position);
            void Clear();
            void Write(const void* data, UInt32 size);
            void Read(void* buffer, UInt32 size);

        private:
            Packet(const Packet& other);
            Packet& operator=(const Packet& other);

            UInt8* _buffer;
            UInt32 _capacity;
            UInt32 _contentLength;
            UInt32 _position;
            bool _failedToStream;
    };

    XpgFunction Packet& operator<<(Packet& packet, const char* data);

    XpgFunction Packet& operator<<(Packet& packet, Int8 data);
    XpgFunction Packet& operator<<(Packet& packet, UInt8 data);
    XpgFunction Packet& operator<<(Packet& packet, Int16 data);
    XpgFunction Packet& operator<<(Packet& packet, UInt16 data);
    XpgFunction Packet& operator<<(Packet& packet, Int32 data);
    XpgFunction Packet& operator<<(Packet& packet, UInt32 data);
    XpgFunction Packet& operator<<(Packet& packet, Int64 data);
    XpgFunction Packet& operator<<(Packet& packet, UInt64 data);

    XpgFunction Packet& operator>>(Packet& packet, Int8& data);
    XpgFunction Packet& operator>>(Packet& packet, UInt8& data);
    XpgFunction Packet& operator>>(Packet& packet, Int16& data);
    XpgFunction Packet& operator>>(Packet& packet, UInt16& data);
    XpgFunction Packet& operator>>(Packet& packet, Int32& data);
    XpgFunction Packet& operator>>(Packet& packet, UInt32& data);
    XpgFunction Packet& operator>>(Packet& packet, Int64& data);
    XpgFunction Packet& operator>>(Packet& packet, UInt64& data);
}

#endif
