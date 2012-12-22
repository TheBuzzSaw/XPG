#ifndef JOYSTICKMANAGER_H
#define JOYSTICKMANAGER_H

#include "Platform.hpp"
#include "DataTypes.hpp"

namespace XPG
{
    XpgClass JoystickManager
    {
        public:
            JoystickManager();
            virtual ~JoystickManager();

            UInt32 GetNumJoysticks();
        protected:
        private:
    };
}

#endif // JOYSTICKMANAGER_H
