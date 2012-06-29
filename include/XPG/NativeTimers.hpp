#ifndef NATIVETIMERS_HPP
#define NATIVETIMERS_HPP

#include "DateTime.hpp"

namespace XPG
{
    void Sleep(TimeSpan timeSpan);
    const DateTime GetNativeTime();
    void ResetTimer();
    const TimeSpan ReadTimer();
}

#endif
