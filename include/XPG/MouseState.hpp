#ifndef XpgMouseStateHpp
#define XpgMouseStateHpp

#include "Platform.hpp"
#include "DataTypes.hpp"

namespace XPG
{
    XpgClass MouseState
    {
        public:
            typedef void (*Callback)(MouseState&);

            MouseState(void* userData, UInt32 buttonFlags, int x, int y);
            MouseState(const MouseState& other);
            ~MouseState();

            MouseState& operator=(const MouseState& other);

            inline int X() const { return _x; }
            inline int Y() const { return _y; }

            bool IsButtonDown(UInt32 button);

        private:
            void* _userData;
            UInt32 _buttonFlags;
            int _x;
            int _y;
    };
}

#endif
