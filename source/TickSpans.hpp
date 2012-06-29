#ifndef TICKSPANS_HPP
#define TICKSPANS_HPP

namespace XPG
{
    const Int64 NanosecondsPerTick = 100;
    const Int64 TicksPerMicrosecond = 10;
    const Int64 TicksPerMillisecond = TicksPerMicrosecond * 1000;
    const Int64 TicksPerSecond = TicksPerMillisecond * 1000;
    const Int64 TicksPerMinute = TicksPerSecond * 60;
    const Int64 TicksPerHour = TicksPerMinute * 60;
    const Int64 TicksPerDay = TicksPerHour * 24;
    const Int64 TicksPerWeek = TicksPerDay * 7;
}

#endif
