#include "../include/XPG/WindowState.hpp"

namespace XPG
{
    WindowState::WindowState()
    {
        _userData = 0;
        _x = 0;
        _y = 0;
        _width = 0;
        _height = 0;
    }

    WindowState::WindowState(const WindowState& other)
    {
        _userData = other._userData;
        _x = other._x;
        _y = other._y;
        _width = other._width;
        _height = other._height;
    }

    WindowState::~WindowState()
    {
    }

    WindowState& WindowState::operator=(const WindowState& other)
    {
        _userData = other._userData;
        _x = other._x;
        _y = other._y;
        _width = other._width;
        _height = other._height;
        return *this;
    }
}
