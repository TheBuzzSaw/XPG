#include "../../include/XPG/JoystickManager.hpp"

#include "Windows.hpp"
#include <Mmsystem.h>

namespace XPG
{
    JoystickManager::JoystickManager()
    {
        //ctor
    }

    JoystickManager::~JoystickManager()
    {
        //dtor
    }

    UInt32 JoystickManager::GetNumJoysticks()
    {
        UInt32 numPossibleJoysticks = joyGetNumDevs();
        return 0;
    }
}
