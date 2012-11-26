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

            inline UInt16 Position() const
            {
                return _position;
            }

            inline UInt16 FreeSpace() const
            {
                return _capacity - _position;
            }

            inline bool FailedToWrite() const
            {
                return _failedToWrite;
            }

            inline operator bool() const
            {
                return _failedToWrite;
            }

            inline void Forgive()
            {
                _failedToWrite = false;
            }

            inline const UInt8* Buffer() const
            {
                return _buffer;
            }

            inline UInt8* Buffer()
            {
                return _buffer;
            }

            void Position(UInt16 position);
            void Clear();
            void Write(const void* data, UInt16 size);

        private:
            Packet32(const Packet32& other);
            Packet32& operator=(const Packet32& other);

            Address32 _address;
            UInt8* _buffer;
            UInt16 _capacity;
            UInt16 _position;
            bool _failedToWrite;
    };
}

#endif
