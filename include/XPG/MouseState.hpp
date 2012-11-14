#ifndef XpgMouseStateHpp
#define XpgMouseStateHpp

#include "Platform.hpp"
#include "DataTypes.hpp"

namespace XPG
{
    XpgClass MouseState
    {
        public:
            enum Button {
                Left = 1 << 0,
                Right = 1 << 1,
                Middle = 1 << 2,
                X1 = 1 << 3,
                X2 = 1 << 4
                };

            MouseState();
            MouseState(const MouseState& other);
            ~MouseState();

            MouseState& operator=(const MouseState& other);

            inline int X() const { return _x; }
            inline int Y() const { return _y; }

            inline void UserData(void* userData) { _userData = userData; }
            inline void X(int x) { _x = x; }
            inline void Y(int y) { _y = y; }

            void SetButton(Button inButton);
            bool const IsButtonDown(const Button inButton);

        private:
            void* _userData;
            UInt32 _buttonFlags;
            int _x;
            int _y;
    };
}

#endif
