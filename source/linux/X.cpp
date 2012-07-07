#include "X.hpp"

namespace XPG
{
    static size_t windowCount = 0;
    static Atom wmDeleteMessage;
    static Display* display = 0;

    void SetupX11()
    {
        display = XOpenDisplay(NULL);
    }

    void CloseX11()
    {
        XCloseDisplay(display);
    }

    Display* GetDisplay()
    {
        return display;
    }

    Atom* GetDeleteMessage()
    {
        return &wmDeleteMessage;
    }

    void AcquireWindow()
    {
        ++windowCount;
    }

    void ReleaseWindow()
    {
        if (windowCount > 0) --windowCount;
    }

    size_t GetWindowCount()
    {
        return windowCount;
    }
}
