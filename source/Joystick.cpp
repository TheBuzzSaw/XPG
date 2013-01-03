#include "../include/XPG/Joystick.hpp"

///This .cpp file is for the generic functions that work
///in all os's (copy constuctor, = operator and destructor
namespace XPG
{
    Joystick::Joystick(const Joystick& joystick)
    {
        _numJoystick = joystick._numJoystick;
        _numAxes = joystick._numAxes;
        _numButtons = joystick._numButtons;
        _numHats = joystick._numHats;

        _axisMinimums = 0;
        _axisMaximums = 0;

        if (joystick._axisMinimums != 0)
        {
            _axisMinimums = new Int32[_numAxes];

            for (UInt32 i = 0; i < _numAxes; ++i)
            {
                _axisMinimums[i] = joystick._axisMinimums[i];
            }
        }

        if (joystick._axisMaximums != 0)
        {
            _axisMaximums = new Int32[_numAxes];

            for (UInt32 i = 0; i < _numAxes; ++i)
            {
                _axisMaximums[i] = joystick._axisMaximums[i];
            }
        }

        _axisStates = new Int32[_numAxes];
        _buttonStates = new bool[_numButtons];
        _hatStates = new Int32[_numHats];

        for (UInt32 i = 0; i < _numAxes; ++i)
        {
            _axisStates[i] = joystick._axisStates[i];
        }

        for (UInt32 i = 0; i < _numButtons; ++i)
        {
            _buttonStates[i] = joystick._buttonStates[i];
        }

        for (UInt32 i = 0; i < _numHats; ++i)
        {
            _hatStates[i] = joystick._hatStates[i];
        }

    }



    Joystick::~Joystick()
    {
        if (_axisMinimums != 0)
            delete [] _axisMinimums;

        if (_axisMaximums != 0)
            delete [] _axisMaximums;

        if (_axisStates != 0)
            delete [] _axisStates;

        if (_buttonStates != 0)
            delete [] _buttonStates;

        if (_hatStates != 0)
            delete [] _hatStates;
    }

    Joystick& Joystick::operator =(const Joystick& joystick)
    {
        _numJoystick = joystick._numJoystick;
        _numAxes = joystick._numAxes;
        _numButtons = joystick._numButtons;
        _numHats = joystick._numHats;

        if (_axisMinimums != 0)
            delete [] _axisMinimums;

        if (_axisMaximums != 0)
            delete [] _axisMaximums;

        if (joystick._axisMinimums != 0)
        {
            _axisMinimums = new Int32[_numAxes];

            for (UInt32 i = 0; i < _numAxes; ++i)
            {
                _axisMinimums[i] = joystick._axisMinimums[i];
            }
        }

        if (joystick._axisMaximums != 0)
        {
            _axisMaximums = new Int32[_numAxes];

            for (UInt32 i = 0; i < _numAxes; ++i)
            {
                _axisMaximums[i] = joystick._axisMaximums[i];
            }
        }

        if (_axisStates != 0)
            delete [] _axisStates;

        if (_buttonStates != 0)
            delete [] _buttonStates;

        if (_hatStates != 0)
            delete [] _hatStates;

        _axisStates = new Int32[_numAxes];
        _buttonStates = new bool[_numButtons];
        _hatStates = new Int32[_numHats];

        for (UInt32 i = 0; i < _numAxes; ++i)
        {
            _axisStates[i] = joystick._axisStates[i];
        }

        for (UInt32 i = 0; i < _numButtons; ++i)
        {
            _buttonStates[i] = joystick._buttonStates[i];
        }

        for (UInt32 i = 0; i < _numHats; ++i)
        {
            _hatStates[i] = joystick._hatStates[i];
        }

        return *this;
    }
}
