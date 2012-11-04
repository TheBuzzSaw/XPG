#include "../include/XPG/MouseState.hpp"
#include <cstring>

namespace XPG
{
    MouseState::MouseState(void* userData, int x, int y)
    {
        _userData = userData;
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

    void MouseState::SetButton(Button inButton)
    {
        _buttonFlags |= inButton;
    }

    bool MouseState::IsButtonDown(Button inButton)
    {
        return _buttonFlags & inButton;
    }
}
