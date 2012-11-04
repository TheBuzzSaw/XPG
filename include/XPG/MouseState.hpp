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

            enum Button {
                Left = 1 << 0,
                Right = 1 << 1,
                Middle = 1 << 2,
                X1 = 1 << 3,
                X2 = 1 << 4 };

            MouseState(void* userData, int x, int y);
            MouseState(const MouseState& other);
            ~MouseState();

            MouseState& operator=(const MouseState& other);

            inline int X() const { return _x; }
            inline int Y() const { return _y; }

            void SetButton(Button inButton);
            bool IsButtonDown(Button inButton);

        private:
            void* _userData;
            UInt32 _buttonFlags;
            int _x;
            int _y;
    };
}

#endif
