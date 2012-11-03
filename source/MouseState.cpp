#include "../include/XPG/MouseState.hpp"
#include <cstring>

namespace XPG
{
    MouseState::MouseState(void* userData, UInt32 buttonFlags, int x, int y)
    {
        _userData = userData;
        _buttonFlags = buttonFlags;
        _x = x;
        _y = y;
    }

    MouseState::MouseState(const MouseState& other)
    {
        // The class consists of primitives. Just copy the memory.
        memcpy(this, &other, sizeof(MouseState));
    }

    MouseState::~MouseState()
    {
    }

    MouseState& MouseState::operator=(const MouseState& other)
    {
        memcpy(this, &other, sizeof(MouseState));
        return *this;
    }

    bool MouseState::IsButtonDown(UInt32 button)
    {
        UInt32 mask = 1 << button;
        return _buttonFlags & mask;
    }
}
