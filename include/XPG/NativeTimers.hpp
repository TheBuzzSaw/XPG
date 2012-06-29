#ifndef XpgNativeTimersHpp
#define XpgNativeTimersHpp

#include "DateTime.hpp"

namespace XPG
{
    XpgFunction void Sleep(TimeSpan timeSpan);
    XpgFunction const DateTime GetNativeTime();
    XpgFunction void ResetTimer();
    XpgFunction const TimeSpan ReadTimer();
}

#endif
