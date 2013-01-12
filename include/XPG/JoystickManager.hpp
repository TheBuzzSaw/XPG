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

            typedef void (*JoystickEventCallback)(UInt32, const Joystick&);

            void ResetAllJoystickInfo();
            UInt32 GetNumJoysticks();

            inline void Deadzone(UInt32 deadzone) { if (deadzone <= 100) _deadzone = (double)deadzone / 100; }

            inline UInt32 Deadzone() const { return (UInt32)(_deadzone * 100); }

            XPG::Joystick* PollJoystickState(UInt32 numJoystick);
            void PollJoystickEvents();

            void OnJoyAxis(JoystickEventCallback callback);
            void OnJoyButtonDown(JoystickEventCallback);
            void OnJoyButtonUp(JoystickEventCallback);
            void OnJoyHat(JoystickEventCallback);
        private:
            UInt32 _numAvailableJoysticks;
            XPG::Joystick** _joysticks;
            double _deadzone;
            UInt8 _native[256];
    };
}

#endif // JOYSTICKMANAGER_H
