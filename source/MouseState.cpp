#include "../include/XPG/MouseState.hpp"
#include <cstring>

namespace XPG
{
    MouseState::MouseState()
    {
        _buttonFlags = 0;
        _userData = NULL;
        _x = 0;
        _y = 0;
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

    void MouseState::SetButton(Button button)
    {
        _buttonFlags |= button;
    }

    bool MouseState::IsButtonDown(Button button) const
    {
        return _buttonFlags & button;
    }
}
