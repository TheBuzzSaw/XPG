#ifndef XpgSocketHpp
#define XpgSocketHpp

#include "../Platform.hpp"
#include "../DataTypes.hpp"

namespace XPG
{
    XpgClass Socket
    {
        public:
            Socket();
            virtual ~Socket();

            virtual bool IsOpen() const = 0;
            virtual void Close() = 0;
            virtual bool SetBlocking(bool blocking) = 0;

        private:
    };
}

#endif
