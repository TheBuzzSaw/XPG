#ifndef XpgCharacterHpp
#define XpgCharacterHpp

#include "Platform.hpp"
#include "DataTypes.hpp"

namespace XPG
{
    XpgClass Character
    {
        public:
            inline Character()
            {
                _value = 0;
            }

            inline Character(Int32 value)
            {
                _value = value;
            }

            inline Character(const Character& other)
            {
                _value = other._value;
            }

            inline ~Character() {}

            Character& operator=(const Character& other);

        private:
            Int32 _value;
    };
}

#endif
