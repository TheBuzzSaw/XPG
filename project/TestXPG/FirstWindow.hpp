#ifndef FIRSTWINDOW_HPP
#define FIRSTWINDOW_HPP

#include <XPG/Window.hpp>

class FirstWindow
{
    public:
        FirstWindow();
        ~FirstWindow();

    private:
        XPG::Window _window;

        static void KeyDown(XPG::Event::Details& details);
        static void Close(XPG::Event::Details& details);
        static void Expose(XPG::Event::Details& details);
};

#endif
