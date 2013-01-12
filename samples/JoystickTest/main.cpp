#include <XPG/Joystick.hpp>
#include <XPG/JoystickManager.hpp>
#include <XPG/Clock.hpp>
#include <XPG/TimeSpan.hpp>
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;


void OnJoyAxis(XPG::UInt32 whichAxis, const XPG::Joystick& joyState)
{
    cout << "Joystick " << joyState.JoystickNum() << " axis " << whichAxis << " value: " << joyState.AxisState(whichAxis) << endl;
}

void OnJoyButtonDown(XPG::UInt32 whichButton, const XPG::Joystick& joyState)
{
    cout << "Joystick " << joyState.JoystickNum() << " button " << whichButton << " pressed." << endl;
}

void OnJoyButtonUp(XPG::UInt32 whichButton, const XPG::Joystick& joyState)
{
    cout << "Joystick " << joyState.JoystickNum() << " button " << whichButton << " release." << endl;
}

void OnJoyHat(XPG::UInt32 whichHat, const XPG::Joystick& joyState)
{
    cout << "Joystick " << joyState.JoystickNum() << " hat " << whichHat << " value: " << joyState.HatState(whichHat) << endl;
}

int main(int argc, char** argv)
{
    unsigned int numJoysticks = XPG::Joystick::NumJoysticks();
    cerr << "Num joysticks: " << numJoysticks << endl;


    XPG::JoystickManager joyManager;
    joyManager.OnJoyAxis(OnJoyAxis);
    joyManager.OnJoyHat(OnJoyHat);
    joyManager.OnJoyButtonDown(OnJoyButtonDown);
    joyManager.OnJoyButtonUp(OnJoyButtonUp);
    joyManager.Deadzone(20);

    while(true)
    {
        joyManager.PollJoystickEvents();
        XPG::Sleep(XPG::TimeSpan::FromMilliseconds(1));
    }


    return 0;
}
