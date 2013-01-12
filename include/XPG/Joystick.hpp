#ifndef XpgJoystickHpp
#define XpgJoystickHpp

#include "Platform.hpp"
#include "DataTypes.hpp"

#define SIZE_OF_JOYSTICK_NATIVE 1024

namespace XPG
{
    XpgClass Joystick
    {
        public:
            Joystick(UInt32 numJoystick);
            Joystick(const Joystick& joystick);
            virtual ~Joystick();

            Joystick& operator =(const Joystick& joystick);

            void PollState();

            inline UInt32 JoystickNum() const { return _numJoystick; }

            static UInt32 NumJoysticks();

            inline UInt32 NumAxes() const { return _numAxes; }
            inline Int32 AxisMinimum(UInt32 axis) const
            {
                Int32 result = 0;
                if (_axisMinimums != 0 && axis < _numAxes)
                    result = _axisMinimums[axis];

                return result;
            }

            inline Int32 AxisMaximum(UInt32 axis) const
            {
                Int32 result = 0;
                if (_axisMaximums != 0 && axis < _numAxes)
                    result = _axisMaximums[axis];

                return result;
            }

            inline UInt32 NumButtons() const { return _numButtons; }
            inline UInt32 NumHats() const { return _numHats; }

            inline const Int32 AxisState(const UInt32 axis) const
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

            inline bool ButtonState(UInt32 button) const
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

            inline Int16 HatState(UInt32 hat) const
            {
                Int16 result = 0;
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
            UInt32 _numJoystick;
            UInt32 _numAxes;
            UInt32 _numButtons;
            UInt32 _numHats;

            Int32* _axisMinimums;
            Int32* _axisMaximums;

            Int32* _axisStates;
            bool* _buttonStates;
            Int16* _hatStates;

            UInt8 _native[SIZE_OF_JOYSTICK_NATIVE];
    };

}

#endif // XpgJoystickHpp
