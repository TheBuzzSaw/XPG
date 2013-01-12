#ifndef XpgJoystickEventBatchHpp
#define XpgJoystickEventBatchHpp

#include "../include/XPG/JoystickManager.hpp"

namespace XPG
{
    struct JoystickEventBatch
    {
        JoystickManager::JoystickEventCallback onJoyAxis;
        JoystickManager::JoystickEventCallback onJoyButtonDown;
        JoystickManager::JoystickEventCallback onJoyButtonUp;
        JoystickManager::JoystickEventCallback onJoyHat;
    };
}


#endif // XpgJoystickEventBatchHpp
