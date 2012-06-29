#include "../../include/XPG/Clock.hpp"
#include "../../include/XPG/DateTime.hpp"
#include "Windows.hpp"

namespace XPG
{
    void Sleep(TimeSpan timeSpan)
    {
        ::Sleep(timeSpan.ToMilliseconds());
    }

    // The Win32 date base is 1601-1-1
    static const Int64 DateBase = 504911232000000000L;

    const DateTime HighResolutionUtcTime()
    {
        FILETIME fileTime;
        GetSystemTimeAsFileTime(&fileTime);

        Int64 result = fileTime.dwHighDateTime;
        result <<= 32;
        result |= fileTime.dwLowDateTime;

        return DateTime(DateBase + result);
    }

    const DateTime HighResolutionLocalTime()
    {
        FILETIME systemFileTime;
        GetSystemTimeAsFileTime(&systemFileTime);

        FILETIME localFileTime;
        FileTimeToLocalFileTime(&systemFileTime, &localFileTime);

        Int64 result = localFileTime.dwHighDateTime;
        result <<= 32;
        result |= localFileTime.dwLowDateTime;

        return DateTime(DateBase + result);
    }

    static TimeSpan timerBase;
    static Int64 frequency = 0;

    static const TimeSpan RawTimer()
    {
        TimeSpan result;

        if (frequency > 0)
        {
            LARGE_INTEGER count;
            QueryPerformanceCounter(&count);
            Int64 microseconds = (count.QuadPart * 1000000) / frequency;
            result = TimeSpan::FromMicroseconds(microseconds);
        }
        else
        {
            result = TimeSpan::FromMilliseconds(GetTickCount());
        }

        return result;
    }

    void ResetTimer()
    {
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        frequency = freq.QuadPart;

        timerBase = RawTimer();
    }

    const TimeSpan ReadTimer()
    {
        return RawTimer() - timerBase;
    }

}
