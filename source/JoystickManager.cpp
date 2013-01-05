#include "../include/XPG/JoystickManager.hpp"

#include "JoystickEventBatch.hpp"

#include <cassert>
#include <iostream>
#include <cstdlib>
#include <cstring>
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

    void ClearJoysticks(XPG::Joystick** joysticks, UInt32 numJoysticks)
    {
        if (joysticks != NULL)
        {
            for (UInt32 i = 0; i < numJoysticks; ++i)
            {
                delete joysticks[i];
            }
            delete [] joysticks;
        }
    }

    JoystickManager::~JoystickManager()
    {
        ClearJoysticks(_joysticks, _numAvailableJoysticks);
    }

    UInt32 JoystickManager::GetNumJoysticks()
    {
        return _numAvailableJoysticks;
    }


    void JoystickManager::ResetAllJoystickInfo()
    {
        _numAvailableJoysticks = XPG::Joystick::NumJoysticks();

        if (_joysticks != NULL)
            ClearJoysticks(_joysticks, _numAvailableJoysticks);

        _joysticks = new XPG::Joystick*[_numAvailableJoysticks];

        for (UInt32 i = 0; i < _numAvailableJoysticks; ++i)
        {
            _joysticks[i] = new XPG::Joystick(i);
        }
    }


    void JoystickManager::PollJoystickEvents()
    {
        JoystickManagerMeta* meta = (JoystickManagerMeta*)_native;

        for (UInt32 i = 0; i < _numAvailableJoysticks; ++i)
        {
            bool joyStateChanged = false;

            XPG::Joystick* oldJoyState = _joysticks[i];

            if (oldJoyState != NULL)
            {
                XPG::Joystick* newJoyState = _joysticks[i];
                newJoyState->PollState();

                if (meta->events.onJoyAxis)
                {
                    for (UInt32 j = 0; j < oldJoyState->NumAxes(); ++j)
                    {
                        Int32 oldAxisState = oldJoyState->AxisState(j);
                        Int32 newAxisState = newJoyState->AxisState(j);

                        UInt32 maxDeadzoneValue = oldJoyState->AxisMaximum(j) * _deadzone;

                        if ((UInt32)abs(newAxisState) <= maxDeadzoneValue)
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
                    _joysticks[i] = newJoyState;
                }

            }
            else
            {
                oldJoyState = new XPG::Joystick(i);
                _joysticks[i] = oldJoyState;
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
