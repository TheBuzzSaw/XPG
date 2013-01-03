#include "../../include/XPG/Joystick.hpp"

#include "../../source/windows/Windows.hpp"

#include <mmsystem.h>
using namespace std;

namespace XPG
{
    /******************
    *   Windows joystick axes go from 0 - 65535.  We want to change this
    *   so that 0 occurs at the axes middle, rather than it's minimum point.
    ******************/
    Int32 AdjustAxisMaxAndMins(UInt32 axisValue, UInt32 maxAxisValue)
    {
        return (Int32)axisValue - ((Int32)maxAxisValue / 2);
    }

    /*************************
    *   Adjusts the axis value when it's read in live from
    *   the joystick.
    *************************/
    Int32 GetAdjustedAxisValue(UInt32 axisValue, UInt32 maxAxisValue)
    {
        return (Int32)(axisValue - maxAxisValue);
    }

    Joystick::Joystick(UInt32 numJoystick)
    {
        _numJoystick = numJoystick;
        JOYCAPS capsInfo;

        _numAxes = 0;
        _numButtons = 0;
        _numHats = 0;


        if (joyGetDevCaps(numJoystick, &capsInfo, sizeof(capsInfo)) == JOYERR_NOERROR)
        {
            _numAxes = capsInfo.wNumAxes;
            _numButtons = capsInfo.wNumButtons;
            _numHats = 0;

            if (capsInfo.wCaps & JOYCAPS_HASPOV)
                _numHats = 1;

            _axisMinimums = NULL;
            _axisMaximums = NULL;

            if (capsInfo.wNumAxes > 0)
            {
                _axisMinimums = new Int32[capsInfo.wNumAxes];
                _axisMaximums = new Int32[capsInfo.wNumAxes];

                switch (capsInfo.wNumAxes)
                {
                    case 6:
                    {
                        _axisMinimums[5] = AdjustAxisMaxAndMins(capsInfo.wVmin, capsInfo.wVmax);
                        _axisMaximums[5] = AdjustAxisMaxAndMins(capsInfo.wVmax, capsInfo.wVmax);
                    }

                    case 5:
                    {
                        _axisMinimums[4] = AdjustAxisMaxAndMins(capsInfo.wUmin, capsInfo.wUmax);
                        _axisMaximums[4] = AdjustAxisMaxAndMins(capsInfo.wUmax, capsInfo.wUmax);
                    }

                    case 4:
                    {
                        _axisMinimums[3] = AdjustAxisMaxAndMins(capsInfo.wRmin, capsInfo.wRmax);
                        _axisMaximums[3] = AdjustAxisMaxAndMins(capsInfo.wRmax, capsInfo.wRmax);
                    }

                    case 3:
                    {
                        _axisMinimums[2] = AdjustAxisMaxAndMins(capsInfo.wZmin, capsInfo.wZmax);
                        _axisMaximums[2] = AdjustAxisMaxAndMins(capsInfo.wZmax, capsInfo.wZmax);
                    }

                    case 2:
                    {
                        _axisMinimums[1] = AdjustAxisMaxAndMins(capsInfo.wYmin, capsInfo.wYmax);
                        _axisMaximums[1] = AdjustAxisMaxAndMins(capsInfo.wYmax, capsInfo.wYmax);
                    }

                    case 1:
                    {
                        _axisMinimums[0] = AdjustAxisMaxAndMins(capsInfo.wXmin, capsInfo.wXmax);
                        _axisMaximums[0] = AdjustAxisMaxAndMins(capsInfo.wXmax, capsInfo.wXmax);
                    }
                }
            }

        }

        _axisStates = new Int32[_numAxes];
        _buttonStates = new bool[_numButtons];
        _hatStates = new Int32[_numHats];
    }




    void ParseJoyInfoEx(XPG::Joystick& joystickToFill, JOYINFOEX& info)
    {
            switch (joystickToFill.NumAxes())
            {
                case 6:
                    joystickToFill.AxisState(5, GetAdjustedAxisValue(info.dwVpos, joystickToFill.AxisMaximum(5)));

                case 5:
                    joystickToFill.AxisState(4, GetAdjustedAxisValue(info.dwUpos, joystickToFill.AxisMaximum(4)));

                case 4:
                    joystickToFill.AxisState(3, GetAdjustedAxisValue(info.dwRpos, joystickToFill.AxisMaximum(3)));

                case 3:
                    joystickToFill.AxisState(2, GetAdjustedAxisValue(info.dwZpos, joystickToFill.AxisMaximum(2)));

                case 2:
                    joystickToFill.AxisState(1, GetAdjustedAxisValue(info.dwYpos, joystickToFill.AxisMaximum(1)));

                case 1:
                    joystickToFill.AxisState(0, GetAdjustedAxisValue(info.dwXpos, joystickToFill.AxisMaximum(0)));
            }

            if (info.dwFlags & JOY_RETURNBUTTONS)
            {
                UInt32 buttonFlag = 0;

                for (UInt32 i = 0; i < joystickToFill.NumButtons(); ++i)
                {
                    buttonFlag = 1 << i;
                    joystickToFill.ButtonState(i, info.dwButtons & buttonFlag);
                }
            }

            switch (joystickToFill.NumHats())
            {
                case 1:
                    joystickToFill.HatState(0, info.dwPOV);
            }
    }

    void Joystick::PollState()
    {
        JOYINFOEX info;
        info.dwSize = sizeof(info);
        info.dwFlags = JOY_RETURNALL;

        if (joyGetPosEx(_numJoystick, &info) == JOYERR_NOERROR)
        {
            ParseJoyInfoEx(*this, info);
        }
    }

    UInt32 Joystick::NumJoysticks()
    {
        UInt32 numPossibleJoysticks = joyGetNumDevs();

        JOYINFOEX info;
        UInt32 numActualJoysticks = 0;
        for (UInt32 i = JOYSTICKID1; i < numPossibleJoysticks; ++i)
        {
            info.dwSize = sizeof(info);
            info.dwFlags = JOY_RETURNALL;

            if (joyGetPosEx(i, &info) == JOYERR_NOERROR)
            {
                ++numActualJoysticks;
            }
            else
            {
                break;
            }
        }

        return numActualJoysticks;
    }

}
