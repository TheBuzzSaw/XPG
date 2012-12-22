#include "../include/XPG/Joystick.hpp"

namespace XPG
{
    Joystick::Joystick(UInt32 numAxes, UInt32 numButtons, UInt32 numHats,
                       Int32* axisMinimums, Int32* axisMaximums)
    {
        _numAxes = numAxes;
        _numButtons = numButtons;
        _numHats = numHats;
        _axisMinimums = axisMinimums;
        _axisMaximums = axisMaximums;

        _axisStates = new Int32[numAxes];
        _buttonStates = new bool[numButtons];
        _hatStates = new Int32[numHats];
    }

    Joystick::~Joystick()
    {
        if (_axisMinimums != NULL)
            delete [] _axisMinimums;

        if (_axisMaximums != NULL)
            delete [] _axisMaximums;

        if (_axisStates != NULL)
            delete [] _axisStates;

        if (_buttonStates != NULL)
            delete [] _buttonStates;

        if (_hatStates != NULL)
            delete [] _hatStates;
    }

}
