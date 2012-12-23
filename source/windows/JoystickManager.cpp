#include "../../include/XPG/JoystickManager.hpp"

#include "Windows.hpp"
#include <mmsystem.h>
#include "../JoystickEventBatch.hpp"

#include <cassert>
#include <iostream>
#include <cstdlib>
using namespace std;

namespace XPG
{

    struct JoystickManagerMeta
    {
        JoystickEventBatch events;
    };

    JoystickManager::JoystickManager()
    {
        _joysticks = NULL;
        ResetAllJoystickInfo();
        assert(sizeof(JoystickManagerMeta) <= sizeof(JoystickManager::_native));
        memset(_native, 0, sizeof(_native));
        _deadzone = 0;
    }

    JoystickManager::~JoystickManager()
    {
        if (_joysticks != NULL)
        {
            for (UInt32 i = 0; i < _numAvailableJoysticks; ++i)
            {
                delete _joysticks[i];
            }
            delete [] _joysticks;
        }
    }

    UInt32 JoystickManager::GetNumJoysticks()
    {
        return _numAvailableJoysticks;
    }

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

    void JoystickManager::ResetAllJoystickInfo()
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

        _numAvailableJoysticks = numActualJoysticks;

        if (_joysticks != NULL)
            delete [] _joysticks;

        _joysticks = new XPG::Joystick*[_numAvailableJoysticks];
        JOYCAPS capsInfo;

        for (UInt32 i = JOYSTICKID1; i < numActualJoysticks; ++i)
        {
            if (joyGetDevCaps(i, &capsInfo, sizeof(capsInfo)) == JOYERR_NOERROR)
            {
                UInt32 numHats = 0;

                if (capsInfo.wCaps & JOYCAPS_HASPOV)
                    numHats = 1;

                Int32* axisMins = NULL;
                Int32* axisMaxs = NULL;

                if (capsInfo.wNumAxes > 0)
                {
                    axisMins = new Int32[capsInfo.wNumAxes];
                    axisMaxs = new Int32[capsInfo.wNumAxes];

                    switch (capsInfo.wNumAxes)
                    {
                        case 6:
                        {
                            axisMins[5] = AdjustAxisMaxAndMins(capsInfo.wVmin, capsInfo.wVmax);
                            axisMaxs[5] = AdjustAxisMaxAndMins(capsInfo.wVmax, capsInfo.wVmax);
                        }

                        case 5:
                        {
                            axisMins[4] = AdjustAxisMaxAndMins(capsInfo.wUmin, capsInfo.wUmax);
                            axisMaxs[4] = AdjustAxisMaxAndMins(capsInfo.wUmax, capsInfo.wUmax);
                        }

                        case 4:
                        {
                            axisMins[3] = AdjustAxisMaxAndMins(capsInfo.wRmin, capsInfo.wRmax);
                            axisMaxs[3] = AdjustAxisMaxAndMins(capsInfo.wRmax, capsInfo.wRmax);
                        }

                        case 3:
                        {
                            axisMins[2] = AdjustAxisMaxAndMins(capsInfo.wZmin, capsInfo.wZmax);
                            axisMaxs[2] = AdjustAxisMaxAndMins(capsInfo.wZmax, capsInfo.wZmax);
                        }

                        case 2:
                        {
                            axisMins[1] = AdjustAxisMaxAndMins(capsInfo.wYmin, capsInfo.wYmax);
                            axisMaxs[1] = AdjustAxisMaxAndMins(capsInfo.wYmax, capsInfo.wYmax);
                        }

                        case 1:
                        {
                            axisMins[0] = AdjustAxisMaxAndMins(capsInfo.wXmin, capsInfo.wXmax);
                            axisMaxs[0] = AdjustAxisMaxAndMins(capsInfo.wXmax, capsInfo.wXmax);
                        }
                    }
                }

                _joysticks[i] = new XPG::Joystick(capsInfo.wNumAxes, capsInfo.wNumButtons, numHats,
                                                  axisMins, axisMaxs);

                cerr << "joystick " << i << " num axes: " << _joysticks[i]->NumAxes() << endl
                    << "num buttons: " << _joysticks[i]->NumButtons() << endl
                    << "num hats: " << _joysticks[i]->NumHats() << endl;

                for (unsigned int j = 0; j < capsInfo.wNumAxes; ++j)
                {
                    cerr << "axis " << j << " min: " << _joysticks[i]->AxisMinimum(j) <<
                        " max: " << _joysticks[i]->AxisMaximum(j) << endl;
                }

                if (capsInfo.wCaps & JOYCAPS_POVCTS)
                    cerr << "joystick " << i << " supports full pov range!" << endl;
            }
            else
            {
                cerr << "joystick " << i << " not found." << endl;
            }
        }
    }

    void ParseJoyInfoEx(XPG::Joystick* joystickToFill, JOYINFOEX& info)
    {
        if (joystickToFill != NULL)
        {

            switch (joystickToFill->NumAxes())
            {
                case 6:
                    joystickToFill->AxisState(5, GetAdjustedAxisValue(info.dwVpos, joystickToFill->AxisMaximum(5)));

                case 5:
                    joystickToFill->AxisState(4, GetAdjustedAxisValue(info.dwUpos, joystickToFill->AxisMaximum(4)));

                case 4:
                    joystickToFill->AxisState(3, GetAdjustedAxisValue(info.dwRpos, joystickToFill->AxisMaximum(3)));

                case 3:
                    joystickToFill->AxisState(2, GetAdjustedAxisValue(info.dwZpos, joystickToFill->AxisMaximum(2)));

                case 2:
                    joystickToFill->AxisState(1, GetAdjustedAxisValue(info.dwYpos, joystickToFill->AxisMaximum(1)));

                case 1:
                    joystickToFill->AxisState(0, GetAdjustedAxisValue(info.dwXpos, joystickToFill->AxisMaximum(0)));
            }

            if (info.dwFlags & JOY_RETURNBUTTONS)
            {
                UInt32 buttonFlag = 0;

                for (UInt32 i = 0; i < joystickToFill->NumButtons(); ++i)
                {
                    buttonFlag = 1 << i;
                    joystickToFill->ButtonState(i, info.dwButtons & buttonFlag);
                }
            }

            switch (joystickToFill->NumHats())
            {
                case 1:
                    joystickToFill->HatState(0, info.dwPOV);
            }
        }
    }

    XPG::Joystick* JoystickManager::PollJoystickState(UInt32 numJoystick)
    {
        XPG::Joystick* joystick = NULL;

        if (numJoystick < _numAvailableJoysticks)
        {
            JOYINFOEX info;
            info.dwSize = sizeof(info);
            info.dwFlags = JOY_RETURNALL;

            if (joyGetPosEx(numJoystick, &info) == JOYERR_NOERROR)
            {
                XPG::Joystick* oldValues = _joysticks[numJoystick];
                Int32* axisMins = new Int32[oldValues->NumAxes()];
                Int32* axisMaxs = new Int32[oldValues->NumAxes()];

                for (UInt32 i = 0; i < oldValues->NumAxes(); ++i)
                {
                    axisMins[i] = oldValues->AxisMinimum(i);
                    axisMaxs[i] = oldValues->AxisMaximum(i);
                }

                XPG::Joystick* newValues = new XPG::Joystick(oldValues->NumAxes(),
                     oldValues->NumButtons(), oldValues->NumHats(), axisMins, axisMaxs);
                ParseJoyInfoEx(newValues, info);

                joystick = newValues;
            }
        }


        return joystick;
    }

    void JoystickManager::PollJoystickEvents()
    {
        JoystickManagerMeta* meta = (JoystickManagerMeta*)_native;

        for (UInt32 i = 0; i < _numAvailableJoysticks; ++i)
        {
            bool joyStateChanged = false;
            XPG::Joystick* newJoyState = PollJoystickState(i);
            XPG::Joystick* oldJoyState = _joysticks[i];

            if (newJoyState != NULL)
            {
                if (oldJoyState != NULL)
                {
                    if (meta->events.onJoyAxis)
                    {
                        for (UInt32 j = 0; j < oldJoyState->NumAxes(); ++j)
                        {
                            Int32 oldAxisState = oldJoyState->AxisState(j);
                            Int32 newAxisState = newJoyState->AxisState(j);
                            if (newAxisState != oldAxisState)
                            {
                                UInt32 maxDeadzoneValue = oldJoyState->AxisMaximum(j) * _deadzone;

                                if (abs(newAxisState) <= maxDeadzoneValue)
                                {
                                    newJoyState->AxisState(j, 0);
                                    newAxisState = 0;
                                }

                                if (newAxisState != oldAxisState)
                                {
                                    joyStateChanged = true;
                                    meta->events.onJoyAxis(i, j, *newJoyState);
                                }
                            }
                        }
                    }

                    if (meta->events.onJoyButtonDown || meta->events.onJoyButtonUp)
                    {
                        for (UInt32 j = 0; j < oldJoyState->NumButtons(); ++j)
                        {
                            if (newJoyState->ButtonState(j) != oldJoyState->ButtonState(j))
                            {
                                joyStateChanged = true;
                                if (newJoyState->ButtonState(j))
                                {
                                    if (meta->events.onJoyButtonDown)
                                        meta->events.onJoyButtonDown(i, j + 1, *newJoyState);
                                }
                                else
                                {
                                    if (meta->events.onJoyButtonUp)
                                        meta->events.onJoyButtonUp(i, j + 1, *newJoyState);
                                }
                            }
                        }
                    }

                    if (meta->events.onJoyHat)
                    {
                        for (UInt32 j = 0; j < oldJoyState->NumHats(); ++j)
                        {
                            if (newJoyState->HatState(j) != oldJoyState->HatState(j))
                            {
                                joyStateChanged = true;
                                meta->events.onJoyHat(i, j, *newJoyState);
                            }
                        }
                    }

                    if (joyStateChanged)
                    {
                        delete _joysticks[i];
                        _joysticks[i] = newJoyState;
                    }
                    else
                    {
                        delete newJoyState;
                    }
                }
                else
                {
                    _joysticks[i] = newJoyState;
                }

            }
        }
    }

    void JoystickManager::OnJoyAxis(JoystickEventCallback callback)
    {
        JoystickManagerMeta* meta = (JoystickManagerMeta*)_native;
        meta->events.onJoyAxis = callback;
    }

    void JoystickManager::OnJoyButtonDown(JoystickEventCallback callback)
    {
        JoystickManagerMeta* meta = (JoystickManagerMeta*)_native;
        meta->events.onJoyButtonDown = callback;
    }

    void JoystickManager::OnJoyButtonUp(JoystickEventCallback callback)
    {
        JoystickManagerMeta* meta = (JoystickManagerMeta*)_native;
        meta->events.onJoyButtonUp = callback;
    }

    void JoystickManager::OnJoyHat(JoystickEventCallback callback)
    {
        JoystickManagerMeta* meta = (JoystickManagerMeta*)_native;
        meta->events.onJoyHat = callback;
    }
}
