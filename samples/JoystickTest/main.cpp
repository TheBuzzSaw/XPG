#include <XPG/Joystick.hpp>
#include <XPG/JoystickManager.hpp>
#include <XPG/Clock.hpp>
#include <XPG/TimeSpan.hpp>
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;


void OnJoyAxis(XPG::UInt32 whichJoystick, XPG::UInt32 whichAxis, const XPG::Joystick& joyState)
{
    cerr << "Joystick axis " << whichAxis << endl;
}

void OnJoyButtonDown(XPG::UInt32 whichJoystick, XPG::UInt32 whichButton, const XPG::Joystick& joyState)
{
    cerr << "Joystick button " << whichButton << endl;
}

int main(int argc, char** argv)
{
    unsigned int numJoysticks = XPG::Joystick::NumJoysticks();
    cerr << "Num joysticks: " << numJoysticks << endl;

//    for (unsigned int i = 0; i < numJoysticks; ++i)
//    {
//        XPG::Joystick joy(i);
//        joy.PollState();
//    }

//    XPG::Joystick joy(2);

    XPG::JoystickManager joyManager;
    joyManager.OnJoyAxis(OnJoyAxis);
    joyManager.OnJoyButtonDown(OnJoyButtonDown);

    while(true)
    {
        joyManager.PollJoystickEvents();
        XPG::Sleep(XPG::TimeSpan::FromMilliseconds(0));
    }


    return 0;
}
