#ifndef XpgWindowStateHpp
#define XpgWindowStateHpp

#include "Platform.hpp"
#include "DataTypes.hpp"

namespace XPG
{
    class XpgApi WindowState
    {
        public:
            WindowState();
            WindowState(const WindowState& other);
            ~WindowState();

            WindowState& operator=(const WindowState& other);

            inline void* UserData() const { return _userData; }
            inline int X() const { return _x; }
            inline int Y() const { return _y; }
            inline int Width() const { return _width; }
            inline int Height() const { return _height; }

            inline void UserData(void* userData) { _userData = userData; }
            inline void X(int x) { _x = x; }
            inline void Y(int y) { _y = y; }
            inline void Width(int width) { _width = width; }
            inline void Height(int height) { _height = height; }

        private:
            void* _userData;
            int _x;
            int _y;
            int _width;
            int _height;
    };
}

#endif
