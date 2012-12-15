#include "../include/XPG/MersenneTwister19937.hpp"
#include <cstring>

namespace XPG
{
    MersenneTwister19937::MersenneTwister19937(int seed)
    {
        _index = 0;
        _values[0] = seed;

        for (int i = 1; i < Size; ++i)
        {
            int previous = _values[i - 1];
            _values[i] = 0x6c078965 * (previous ^ (previous >> 30)) + i;
        }
    }

    MersenneTwister19937::MersenneTwister19937(const MersenneTwister19937&
        other)
    {
        memcpy(_values, other._values, sizeof(_values));
        _index = other._index;
    }

    MersenneTwister19937::~MersenneTwister19937()
    {
    }

    MersenneTwister19937& MersenneTwister19937::operator=(const
        MersenneTwister19937& other)
    {
        memcpy(_values, other._values, sizeof(_values));
        _index = other._index;
        return *this;
    }

    int MersenneTwister19937::Next()
    {
        if (_index == 0)
        {
            for (int i = 0; i < Size; ++i)
            {
                int n = (_values[i] & 0x80000000)
                    + (_values[(i + 1) % Size] & 0x7fffffff);

                _values[i] = _values[(i + 397) % Size] ^ (n >> 1);

                if (n & 1) _values[i] ^= 0x9908b0df;
            }
        }

        int p = _values[_index];
        p ^= p >> 11;
        p ^= (p << 7) & 0x9d2c5680;
        p ^= (p << 15) & 0xefc60000;
        p ^= p >> 18;

        _index = (_index + 1) % Size;

        return p;
    }
}
