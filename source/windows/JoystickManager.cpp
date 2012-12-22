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

        JOYINFO info;
        UInt32 numActualJoysticks = 0;
        for (; numActualJoysticks < numPossibleJoysticks; ++numActualJoysticks)
        {
            if (joyGetPos(numActualJoysticks, &info) != JOYERR_NOERROR)
                break;
        }
        return numActualJoysticks;
    }
}
