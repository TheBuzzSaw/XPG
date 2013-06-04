#include "../../include/XPG/Clock.hpp"
#include <ctime>
#include <unistd.h>

#ifdef CLOCK_MONOTONIC_RAW
#   define XpgClockMonotonic CLOCK_MONOTONIC_RAW
#elif defined(CLOCK_MONOTONIC)
#   define XpgClockMonotonic CLOCK_MONOTONIC
#else
#   error No available clock macro!
#endif

namespace XPG
{
    static const DateTime Epoch(1970, 1, 1);

    void Sleep(TimeSpan timeSpan)
    {
#ifndef XpgPlatformAndroid
        usleep(timeSpan.ToMicroseconds());
#endif
    }

    const DateTime HighResolutionUtcTime()
    {
        timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);

        return Epoch
            + TimeSpan::FromSeconds(ts.tv_sec)
            + TimeSpan::FromNanoseconds(ts.tv_nsec);
    }

    const DateTime HighResolutionLocalTime()
    {
        // Need to convert the HR UTC time to local time. =/
        return DateTime::LocalTime();
    }

    static TimeSpan timerBase;

    const TimeSpan RawTimer()
    {
        timespec ts;
        clock_gettime(XpgClockMonotonic, &ts);

        return TimeSpan::FromSeconds(ts.tv_sec)
            + TimeSpan::FromNanoseconds(ts.tv_nsec);
    }

    void ResetTimer()
    {
        timerBase = RawTimer();
    }

    const TimeSpan ReadTimer()
    {
        return RawTimer() - timerBase;
    }
}
