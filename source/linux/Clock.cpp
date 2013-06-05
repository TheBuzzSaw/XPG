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
        const Int64 NanoUnits = 1000000000;
        Int64 nanoseconds = timeSpan.ToNanoseconds();
        Int64 seconds = nanoseconds / NanoUnits;
        nanoseconds -= seconds * NanoUnits;

        timespec ts;
        ts.tv_sec = seconds;
        ts.tv_nsec = nanoseconds;
        while (nanosleep(&ts, &ts) == -1);
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
