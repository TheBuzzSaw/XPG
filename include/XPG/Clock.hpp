#ifndef XpgClockHpp
#define XpgClockHpp

#include "DateTime.hpp"

namespace XPG
{
    XpgFunction void Sleep(TimeSpan duration);
    XpgFunction const DateTime HighResolutionUtcTime();
    XpgFunction const DateTime HighResolutionLocalTime();
    XpgFunction void ResetTimer();
    XpgFunction const TimeSpan ReadTimer();
}

#endif
