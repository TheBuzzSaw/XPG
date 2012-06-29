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

        return DateTime(1970, 1, 1)
            + TimeSpan::FromSeconds(ts.tv_sec)
            + TimeSpan::FromNanoseconds(ts.tv_nsec);
    }

    static TimeSpan timerBase;

    const TimeSpan RawTimer()
    {
        timespec ts;
        clock_gettime(CLOCK_MONOTONIC_RAW, &ts);

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
