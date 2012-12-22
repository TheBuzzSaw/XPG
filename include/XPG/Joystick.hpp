#ifndef XpgJoystickHpp
#define XpgJoystickHpp

#include "../../source/windows/Windows.hpp"
#include "Platform.hpp"
#include "DataTypes.hpp"

namespace XPG
{
    XpgClass Joystick
    {
        public:
            Joystick(UInt32 numAxes, UInt32 numButtons, UInt32 numHats,
                     Int32* axisMinimums, Int32* axisMaximums);
            virtual ~Joystick();

            inline UInt32 NumAxes() { return _numAxes; }
            inline Int32 AxisMinimum(UInt32 axis)
            {
                Int32 result = 0;
                if (_axisMinimums != NULL && axis < _numAxes)
                    result = _axisMinimums[axis];

                return result;
            }

            inline Int32 AxisMaximum(UInt32 axis)
            {
                Int32 result = 0;
                if (_axisMaximums != NULL && axis < _numAxes)
                    result = _axisMaximums[axis];

                return result;
            }

            inline UInt32 NumButtons() { return _numButtons; }
            inline UInt32 NumHats() { return _numHats; }

            inline Int32 AxisState(UInt32 axis)
            {
                Int32 result = 0;
                if (axis < _numAxes)
                    result = _axisStates[axis];
                return result;
            }

            inline void AxisState(UInt32 axis, Int32 state)
            {
                if (axis < _numAxes)
                    _axisStates[axis] = state;
            }

            inline bool ButtonState(UInt32 button)
            {
                bool result = false;
                if (button < _numButtons)
                    result = _buttonStates[button];
                return result;
            }

            inline void ButtonState(UInt32 button, bool state)
            {
                if (button < _numButtons)
                    _buttonStates[button] = state;
            }

            inline Int32 HatState(UInt32 hat)
            {
                Int32 result = 0;
                if (hat < _numHats)
                    result = _hatStates[hat];
                return result;
            }

            inline void HatState(UInt32 hat, Int32 state)
            {
                if (hat < _numHats)
                    _hatStates[hat] = state;
            }


        private:
            UInt32 _numAxes;
            UInt32 _numButtons;
            UInt32 _numHats;
            Int32* _axisMinimums;
            Int32* _axisMaximums;

            Int32* _axisStates;
            bool* _buttonStates;
            Int32* _hatStates;
    };

}

#endif // XpgJoystickHpp
