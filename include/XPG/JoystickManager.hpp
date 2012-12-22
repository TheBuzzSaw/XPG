#ifndef JOYSTICKMANAGER_H
#define JOYSTICKMANAGER_H

#include "Platform.hpp"
#include "DataTypes.hpp"
#include "Joystick.hpp"

namespace XPG
{
    XpgClass JoystickManager
    {
        public:
            JoystickManager();
            virtual ~JoystickManager();

            void ResetAllJoystickInfo();
            UInt32 GetNumJoysticks();

            XPG::Joystick* PollJoystickState(UInt32 numJoystick);
        protected:
        private:
            UInt32 mNumAvailableJoysticks;
            XPG::Joystick** mJoysticks;
    };
}

#endif // JOYSTICKMANAGER_H
