#include "../../include/XPG/DateTime.hpp"
#include <ctime>

namespace XPG
{
    void Sleep(TimeSpan timeSpan)
    {
        usleep(timeSpan.ToMicroseconds());
    }

    const DateTime GetNativeTime()
    {
        timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);

        Int64 nanoseconds = ts.tv_nsec;
        nanoseconds += ts.tv_sec * 1000000000;

        return DateTime(1970, 1, 1) + TimeSpan::FromNanoseconds(nanoseconds);
    }

    static TimeSpan timerBase;

    const TimeSpan RawTimer()
    {
        timespec ts;
        clock_gettime(CLOCK_MONOTONIC_RAW, &ts);

        Int64 nanoseconds = ts.tv_nsec;
        nanoseconds += ts.tv_sec * 1000000000;

        return TimeSpan::FromNanoseconds(nanoseconds);
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
