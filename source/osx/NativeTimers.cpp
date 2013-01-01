#include "../../include/XPG/Clock.hpp"
#include <sys/time.h>
#include <mach/mach_time.h>

namespace XPG
{
    void Sleep(TimeSpan duration)
    {
        timespec ts;
        ts.tv_sec = duration.ToSeconds();
        ts.tv_nsec = duration.ToNanoseconds() % 1000000000;
        nanosleep(&ts, &ts);
    }
    
    const DateTime HighResolutionLocalTime()
    {
        timeval tv;
        gettimeofday(&tv, NULL);
        int64_t microseconds = tv.tv_sec * 1000000;
        microseconds += tv.tv_usec;
        
        return DateTime(1970, 1, 1) + TimeSpan::FromMicroseconds(microseconds);
    }
    
    static uint64_t timerBase = 0;
    static mach_timebase_info_data_t timeBaseInfo;
    
    void ResetTimer()
    {
        mach_timebase_info(&timeBaseInfo);
        timerBase = mach_absolute_time();
    }
    
    const TimeSpan ReadTimer()
    {
        uint64_t end = mach_absolute_time();
        uint64_t elapsed = end - timerBase;
        uint64_t nanoseconds = elapsed * timeBaseInfo.numer
            / timeBaseInfo.denom;
        return TimeSpan::FromNanoseconds(nanoseconds);
    }
}
