#include "../../include/XPG/JoystickManager.hpp"

#include "Windows.hpp"
#include <Mmsystem.h>

#include <iostream>
using namespace std;

namespace XPG
{
    JoystickManager::JoystickManager()
    {
        mJoysticks = NULL;
        ResetAllJoystickInfo();
    }

    JoystickManager::~JoystickManager()
    {
        if (mJoysticks != NULL)
        {
            for (UInt32 i = 0; i < mNumAvailableJoysticks; ++i)
            {
                delete mJoysticks[i];
            }
            delete [] mJoysticks;
        }
    }

    UInt32 JoystickManager::GetNumJoysticks()
    {
        return mNumAvailableJoysticks;
    }

    /******************
    *   Windows joystick axes go from 0 - 65535.  We want to change this
    *   so that 0 occurs at the axes middle, rather than it's minimum point.
    ******************/
    Int32 GetAxisValue(UInt32 axisValue, UInt32 maxAxisValue)
    {
        return (Int32)axisValue - ((Int32)maxAxisValue / 2);
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

        mNumAvailableJoysticks = numActualJoysticks;

        if (mJoysticks != NULL)
            delete [] mJoysticks;

        mJoysticks = new XPG::Joystick*[mNumAvailableJoysticks];
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
                            axisMins[5] = GetAxisValue(capsInfo.wVmin, capsInfo.wVmax);
                            axisMaxs[5] = GetAxisValue(capsInfo.wVmax, capsInfo.wVmax);
                        }

                        case 5:
                        {
                            axisMins[4] = GetAxisValue(capsInfo.wUmin, capsInfo.wUmax);
                            axisMaxs[4] = GetAxisValue(capsInfo.wUmax, capsInfo.wUmax);
                        }

                        case 4:
                        {
                            axisMins[3] = GetAxisValue(capsInfo.wRmin, capsInfo.wRmax);
                            axisMaxs[3] = GetAxisValue(capsInfo.wRmax, capsInfo.wRmax);
                        }

                        case 3:
                        {
                            axisMins[2] = GetAxisValue(capsInfo.wZmin, capsInfo.wZmax);
                            axisMaxs[2] = GetAxisValue(capsInfo.wZmax, capsInfo.wZmax);
                        }

                        case 2:
                        {
                            axisMins[1] = GetAxisValue(capsInfo.wYmin, capsInfo.wYmax);
                            axisMaxs[1] = GetAxisValue(capsInfo.wYmax, capsInfo.wYmax);
                        }

                        case 1:
                        {
                            axisMins[0] = GetAxisValue(capsInfo.wXmin, capsInfo.wXmax);
                            axisMaxs[0] = GetAxisValue(capsInfo.wXmax, capsInfo.wXmax);
                        }
                    }
                }

                mJoysticks[i] = new XPG::Joystick(capsInfo.wNumAxes, capsInfo.wNumButtons, numHats,
                                                  axisMins, axisMaxs);

                cerr << "joystick " << i << " num axes: " << mJoysticks[i]->NumAxes() << endl
                    << "num buttons: " << mJoysticks[i]->NumButtons() << endl
                    << "num hats: " << mJoysticks[i]->NumHats() << endl;

                for (unsigned int j = 0; j < capsInfo.wNumAxes; ++j)
                {
                    cerr << "axis " << j << " min: " << mJoysticks[i]->AxisMinimum(j) <<
                        " max: " << mJoysticks[i]->AxisMaximum(j) << endl;
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
                    joystickToFill->AxisState(5, GetAxisValue(info.dwVpos, joystickToFill->AxisMaximum(5)));

                case 5:
                    joystickToFill->AxisState(4, GetAxisValue(info.dwUpos, joystickToFill->AxisMaximum(4)));

                case 4:
                    joystickToFill->AxisState(3, GetAxisValue(info.dwRpos, joystickToFill->AxisMaximum(3)));

                case 3:
                    joystickToFill->AxisState(2, GetAxisValue(info.dwZpos, joystickToFill->AxisMaximum(2)));

                case 2:
                    joystickToFill->AxisState(1, GetAxisValue(info.dwYpos, joystickToFill->AxisMaximum(1)));

                case 1:
                    joystickToFill->AxisState(0, GetAxisValue(info.dwXpos, joystickToFill->AxisMaximum(0)));
            }

            switch (joystickToFill->NumButtons())
            {
                case 32:
                    joystickToFill->ButtonState(31, info.dwButtons & JOY_BUTTON32);

                case 31:
                    joystickToFill->ButtonState(30, info.dwButtons & JOY_BUTTON31);

                case 30:
                    joystickToFill->ButtonState(29, info.dwButtons & JOY_BUTTON30);

                case 29:
                    joystickToFill->ButtonState(28, info.dwButtons & JOY_BUTTON29);

                case 28:
                    joystickToFill->ButtonState(27, info.dwButtons & JOY_BUTTON28);

                case 27:
                    joystickToFill->ButtonState(26, info.dwButtons & JOY_BUTTON27);

                case 26:
                    joystickToFill->ButtonState(25, info.dwButtons & JOY_BUTTON26);

                case 25:
                    joystickToFill->ButtonState(24, info.dwButtons & JOY_BUTTON25);

                case 24:
                    joystickToFill->ButtonState(23, info.dwButtons & JOY_BUTTON24);

                case 23:
                    joystickToFill->ButtonState(22, info.dwButtons & JOY_BUTTON23);

                case 22:
                    joystickToFill->ButtonState(21, info.dwButtons & JOY_BUTTON22);

                case 21:
                    joystickToFill->ButtonState(20, info.dwButtons & JOY_BUTTON21);

                case 20:
                    joystickToFill->ButtonState(19, info.dwButtons & JOY_BUTTON20);

                case 19:
                    joystickToFill->ButtonState(18, info.dwButtons & JOY_BUTTON19);

                case 18:
                    joystickToFill->ButtonState(17, info.dwButtons & JOY_BUTTON18);

                case 17:
                    joystickToFill->ButtonState(16, info.dwButtons & JOY_BUTTON17);

                case 16:
                    joystickToFill->ButtonState(15, info.dwButtons & JOY_BUTTON16);

                case 15:
                    joystickToFill->ButtonState(14, info.dwButtons & JOY_BUTTON15);

                case 14:
                    joystickToFill->ButtonState(13, info.dwButtons & JOY_BUTTON14);

                case 13:
                    joystickToFill->ButtonState(12, info.dwButtons & JOY_BUTTON13);

                case 12:
                    joystickToFill->ButtonState(11, info.dwButtons & JOY_BUTTON12);

                case 11:
                    joystickToFill->ButtonState(10, info.dwButtons & JOY_BUTTON11);

                case 10:
                    joystickToFill->ButtonState(9, info.dwButtons & JOY_BUTTON10);

                case 9:
                    joystickToFill->ButtonState(8, info.dwButtons & JOY_BUTTON9);

                case 8:
                    joystickToFill->ButtonState(7, info.dwButtons & JOY_BUTTON8);

                case 7:
                    joystickToFill->ButtonState(6, info.dwButtons & JOY_BUTTON7);

                case 6:
                    joystickToFill->ButtonState(5, info.dwButtons & JOY_BUTTON6);

                case 5:
                    joystickToFill->ButtonState(4, info.dwButtons & JOY_BUTTON5);

                case 4:
                    joystickToFill->ButtonState(3, info.dwButtons & JOY_BUTTON4);

                case 3:
                    joystickToFill->ButtonState(2, info.dwButtons & JOY_BUTTON3);

                case 2:
                    joystickToFill->ButtonState(1, info.dwButtons & JOY_BUTTON2);

                case 1:
                    joystickToFill->ButtonState(0, info.dwButtons & JOY_BUTTON1);
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

        if (numJoystick < mNumAvailableJoysticks)
        {
            JOYINFOEX info;
            info.dwSize = sizeof(info);
            info.dwFlags = JOY_RETURNALL;

            if (joyGetPosEx(numJoystick, &info) == JOYERR_NOERROR)
            {
                XPG::Joystick* oldValues = mJoysticks[numJoystick];
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

}
