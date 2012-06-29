#include "../include/XPG/TimeSpan.hpp"
#include "TickSpans.hpp"

namespace XPG
{
    TimeSpan::TimeSpan() : _ticks(0)
    {
    }

    TimeSpan::TimeSpan(Int64 ticks) : _ticks(ticks)
    {
    }

    TimeSpan::TimeSpan(const TimeSpan& timeSpan) : _ticks(timeSpan._ticks)
    {
    }

    TimeSpan::~TimeSpan()
    {
    }

    TimeSpan& TimeSpan::operator=(const TimeSpan& timeSpan)
    {
        _ticks = timeSpan._ticks;
        return *this;
    }

    TimeSpan& TimeSpan::operator+=(const TimeSpan& timeSpan)
    {
        _ticks += timeSpan._ticks;
        return *this;
    }

    TimeSpan& TimeSpan::operator-=(const TimeSpan& timeSpan)
    {
        _ticks -= timeSpan._ticks;
        return *this;
    }

    bool TimeSpan::operator==(const TimeSpan& timeSpan) const
    {
        return _ticks == timeSpan._ticks;
    }

    bool TimeSpan::operator!=(const TimeSpan& timeSpan) const
    {
        return _ticks != timeSpan._ticks;
    }

    bool TimeSpan::operator<(const TimeSpan& timeSpan) const
    {
        return _ticks < timeSpan._ticks;
    }

    bool TimeSpan::operator<=(const TimeSpan& timeSpan) const
    {
        return _ticks <= timeSpan._ticks;
    }

    bool TimeSpan::operator>(const TimeSpan& timeSpan) const
    {
        return _ticks > timeSpan._ticks;
    }

    bool TimeSpan::operator>=(const TimeSpan& timeSpan) const
    {
        return _ticks >= timeSpan._ticks;
    }

    const TimeSpan TimeSpan::operator+(const TimeSpan& timeSpan) const
    {
        return TimeSpan(_ticks + timeSpan._ticks);
    }

    const TimeSpan TimeSpan::operator-(const TimeSpan& timeSpan) const
    {
        return TimeSpan(_ticks - timeSpan._ticks);
    }

    const TimeSpan TimeSpan::operator-() const
    {
        return TimeSpan(-_ticks);
    }

    Int64 TimeSpan::ToNanoseconds() const
    {
        return _ticks * NanosecondsPerTick;
    }

    Int64 TimeSpan::ToMicroseconds() const
    {
        return _ticks / TicksPerMicrosecond;
    }

    Int64 TimeSpan::ToMilliseconds() const
    {
        return _ticks / TicksPerMillisecond;
    }

    Int64 TimeSpan::ToSeconds() const
    {
        return _ticks / TicksPerSecond;
    }

    Int64 TimeSpan::ToMinutes() const
    {
        return _ticks / TicksPerMinute;
    }

    Int64 TimeSpan::ToHours() const
    {
        return _ticks / TicksPerHour;
    }

    Int64 TimeSpan::ToDays() const
    {
        return _ticks / TicksPerDay;
    }

    Int64 TimeSpan::ToWeeks() const
    {
        return _ticks / TicksPerWeek;
    }

    const TimeSpan TimeSpan::FromNanoseconds(Int64 nanoseconds)
    {
        return TimeSpan(nanoseconds / NanosecondsPerTick);
    }

    const TimeSpan TimeSpan::FromMicroseconds(Int64 microseconds)
    {
        return TimeSpan(microseconds * TicksPerMicrosecond);
    }

    const TimeSpan TimeSpan::FromMilliseconds(Int64 milliseconds)
    {
        return TimeSpan(milliseconds * TicksPerMillisecond);
    }

    const TimeSpan TimeSpan::FromSeconds(Int64 seconds)
    {
        return TimeSpan(seconds * TicksPerSecond);
    }

    const TimeSpan TimeSpan::FromMinutes(Int64 minutes)
    {
        return TimeSpan(minutes * TicksPerMinute);
    }

    const TimeSpan TimeSpan::FromHours(Int64 hours)
    {
        return TimeSpan(hours * TicksPerHour);
    }

    const TimeSpan TimeSpan::FromDays(Int64 days)
    {
        return TimeSpan(days * TicksPerDay);
    }

    const TimeSpan TimeSpan::FromWeeks(Int64 weeks)
    {
        return TimeSpan(weeks * TicksPerWeek);
    }

}
