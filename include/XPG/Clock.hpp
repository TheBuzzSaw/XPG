#ifndef XpgClockHpp
#define XpgClockHpp

#include "DateTime.hpp"

namespace XPG
{
    XpgApi void Sleep(TimeSpan timeSpan);
    XpgApi const DateTime HighResolutionUtcTime();
    XpgApi const DateTime HighResolutionLocalTime();
    XpgApi void ResetTimer();
    XpgApi const TimeSpan ReadTimer();
}

#endif
