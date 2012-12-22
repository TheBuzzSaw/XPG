#ifndef JOYSTICKMANAGER_H
#define JOYSTICKMANAGER_H

#include "DataTypes.hpp"

namespace XPG
{
    class JoystickManager
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
