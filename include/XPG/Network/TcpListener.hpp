#ifndef XpgTcpListenerHpp
#define XpgTcpListenerHpp

#include "../Platform.hpp"
#include "../DataTypes.hpp"

namespace XPG
{
    XpgClass TcpListener
    {
        public:
            friend class TcpSocket;

            TcpListener();
            ~TcpListener();

            bool Open(UInt16 port);
            bool IsOpen() const;
            void Close();

        private:
            UInt8 _native[16];
    };
}

#endif
