#include "../../include/XPG/Window.hpp"
#include <cstring>

namespace XPG
{
    Window::Window()
    {
        memset(_native, 0, sizeof(_native));
        Open();
    }

    Window::~Window()
    {
    }

    void Window::Open()
    {
    }


}
