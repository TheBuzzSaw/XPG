#include "FirstWindow.hpp"
#include <XPG/Key.hpp>
#include <XPG/glew.h>
#include <iostream>

FirstWindow::FirstWindow()
{
    _window.SetTitle("XPG Main Window");
    _window.OnKeyDown().Aim(KeyDown, this);
    _window.OnExpose().Aim(Expose, this);
    _window.OnClose().Aim(Close, this);
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
}

FirstWindow::~FirstWindow()
{
}

void FirstWindow::KeyDown(XPG::Event::Details& details)
{
    std::cout << "KeyDown" << std::endl;

    XPG::Key::Code* key = (XPG::Key::Code*)details.data;

    if (*key == XPG::Key::Escape)
    {
        FirstWindow* fw = (FirstWindow*)details.userData;
        fw->_window.Close();
    }
}

void FirstWindow::Close(XPG::Event::Details& details)
{
    //FirstWindow* fw = (FirstWindow*)details.userData;
    std::cout << "closed FirstWindow" << std::endl;
}

void FirstWindow::Expose(XPG::Event::Details& details)
{
    FirstWindow* fw = (FirstWindow*)details.userData;
    fw->_window.MakeCurrent();
    glClear(GL_COLOR_BUFFER_BIT);
    fw->_window.SwapBuffers();
}
