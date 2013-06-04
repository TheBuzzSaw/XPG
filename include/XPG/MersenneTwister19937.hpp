#ifndef XpgMersenneTwister19937Hpp
#define XpgMersenneTwister19937Hpp

#include "Platform.hpp"

// http://en.wikipedia.org/wiki/Mersenne_twister#Pseudocode

namespace XPG
{
    class XpgApi MersenneTwister19937
    {
        public:
            MersenneTwister19937(int seed);
            MersenneTwister19937(const MersenneTwister19937& other);
            ~MersenneTwister19937();

            MersenneTwister19937& operator=(const MersenneTwister19937& other);

            int Next();

        protected:
        private:
            static const int Size = 624;

            int _values[Size];
            int _index;
    };
}

#endif
