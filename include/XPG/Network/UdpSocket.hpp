#ifndef XpgUdpSocketHpp
#define XpgUdpSocketHpp

#include "Address32.hpp"

namespace XPG
{
    XpgClass UdpSocket
    {
        public:
            UdpSocket();
            ~UdpSocket();

            bool Open(UInt16 port);
            void Close();
            bool IsOpen() const;
            bool Send(const Address32& destination, const void* data,
                int size) const;
            int Receive(Address32& source, void* data, int size) const;

        private:
            UdpSocket(const UdpSocket& other);
            UdpSocket& operator=(const UdpSocket& other);

            UInt8 _native[16];
    };
}

#endif
