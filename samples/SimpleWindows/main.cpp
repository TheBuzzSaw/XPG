#include <XPG/Event.hpp>
#include <XPG/Stopwatch.hpp>
#include <XPG/Clock.hpp>
#include <XPG/Thread.hpp>
#include <XPG/Mutex.hpp>
#include <XPG/Window.hpp>
#include <XPG/Application.hpp>
#include <XPG/Array.hpp>
#include <XPG/Key.hpp>
#include <iostream>
#include <string>
using namespace std;

void PrintMouseState(XPG::MouseState inState)
{
    cout << "Current buttons pressed on mouse: ";
    if (inState.IsButtonDown(XPG::MouseState::Left))
        cout << " left";

    if (inState.IsButtonDown(XPG::MouseState::Right))
        cout << " right";

    if (inState.IsButtonDown(XPG::MouseState::Middle))
        cout << " middle";

    if (inState.IsButtonDown(XPG::MouseState::X1))
        cout << " X1";

    if (inState.IsButtonDown(XPG::MouseState::X2))
        cout << " X2";

    cout << endl;
}

void OnLeftMouseButtonDown(const XPG::MouseState& currentState)
{
    cerr << "Hello!!!! x,y: " << currentState.X() << ", " << currentState.Y()
        << endl;
    PrintMouseState(currentState);
}

void OnMouseMove(const XPG::MouseState& currentState)
{
    cerr << "Mouse Moved! x,y: " << currentState.X() << ", "
        << currentState.Y() << endl;
    PrintMouseState(currentState);
}

void OnMouseExtraButtonDown(const XPG::MouseState& currentState, int inWhichX)
{
    cerr << "Mouse X button down.  Button: " << inWhichX << endl;
    PrintMouseState(currentState);
}

void OnRightMouseButtonDown(const XPG::MouseState& currentState)
{
    PrintMouseState(currentState);
}

void OnMouseWheel(const XPG::MouseState& currentState)
{
    PrintMouseState(currentState);
}

void OnKeyDown(XPG::Key::Code inKey, void* data)
{
    if (inKey == XPG::Key::LetterA)
    {
        cout << "Letter A pressed!" << endl;
    }
    else if (inKey == XPG::Key::CapsLock)
    {
        cout << "Caps Lock Down" << endl;
    }
}

void OnKeyUp(XPG::Key::Code inKey, void* data)
{
    if (inKey == XPG::Key::LetterA)
    {
        cout << "Letter A up!" << endl;
    }
    else if (inKey == XPG::Key::CapsLock)
    {
        cout << "Caps Lock Up" << endl;
    }
}

void OnMouseOut(void* data)
{
    cout << "goodbye" << endl;
}

void OnMouseIn(void* data)
{
    cout << "WELCOME BACK" << endl;
}

void OnExpose(void* data)
{
    static int x = 0;
    cout << "EXPOSE " << ++x << endl;
}

bool OnWindowClose(void* inUserData)
{
    return true;
}

int main(int argc, char** argv)
{
    XPG::Application application;
    XPG::Window window[3];
    window[0].SetTitle("XPG Main Window");
    window[1].SetTitle("XPG Mini Map");

    window[0].OnLeftMouseButtonDown(OnLeftMouseButtonDown);
    //window[0].OnMouseMove(OnMouseMove);
    window[0].OnMouseExtraButtonDown(OnMouseExtraButtonDown);
    window[0].OnRightMouseButtonDown(OnRightMouseButtonDown);
    window[0].OnMouseWheelUp(OnMouseWheel);
    window[0].OnKeyDown(OnKeyDown);
    window[0].OnKeyUp(OnKeyUp);
    window[0].OnClose(OnWindowClose);
    window[2].OnExpose(OnExpose);
    window[0].OnMouseOut(OnMouseOut);
    window[0].OnMouseIn(OnMouseIn);
    application.Run();

    return 0;
}
