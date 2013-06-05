#ifndef XpgTimeSpanHpp
#define XpgTimeSpanHpp

#include "Platform.hpp"
#include "DataTypes.hpp"

namespace XPG
{
    class XpgApi TimeSpan
    {
        public:
            TimeSpan();
            explicit TimeSpan(Int64 ticks);
            TimeSpan(const TimeSpan& other);
            ~TimeSpan();

            inline Int64 Ticks() const { return _ticks; }

            TimeSpan& operator=(const TimeSpan& other);
            TimeSpan& operator+=(const TimeSpan& other);
            TimeSpan& operator-=(const TimeSpan& other);
            TimeSpan& operator/=(Int64 operand);

            bool operator==(const TimeSpan& other) const;
            bool operator!=(const TimeSpan& other) const;
            bool operator<(const TimeSpan& other) const;
            bool operator<=(const TimeSpan& other) const;
            bool operator>(const TimeSpan& other) const;
            bool operator>=(const TimeSpan& other) const;

            const TimeSpan operator+(const TimeSpan& other) const;
            const TimeSpan operator-(const TimeSpan& other) const;
            const TimeSpan operator/(Int64 operand) const;
            const TimeSpan operator-() const;

            Int64 ToNanoseconds() const;
            Int64 ToMicroseconds() const;
            Int64 ToMilliseconds() const;
            Int64 ToSeconds() const;
            Int64 ToMinutes() const;
            Int64 ToHours() const;
            Int64 ToDays() const;
            Int64 ToWeeks() const;

            static const TimeSpan FromNanoseconds(Int64 nanoseconds);
            static const TimeSpan FromMicroseconds(Int64 microseconds);
            static const TimeSpan FromMilliseconds(Int64 milliseconds);
            static const TimeSpan FromSeconds(Int64 seconds);
            static const TimeSpan FromMinutes(Int64 minutes);
            static const TimeSpan FromHours(Int64 hours);
            static const TimeSpan FromDays(Int64 days);
            static const TimeSpan FromWeeks(Int64 weeks);

            static const Int64 NanosecondsPerTick = 100;
            static const Int64 TicksPerMicrosecond = 10;
            static const Int64 TicksPerMillisecond = TicksPerMicrosecond * 1000;
            static const Int64 TicksPerSecond = TicksPerMillisecond * 1000;
            static const Int64 TicksPerMinute = TicksPerSecond * 60;
            static const Int64 TicksPerHour = TicksPerMinute * 60;
            static const Int64 TicksPerDay = TicksPerHour * 24;
            static const Int64 TicksPerWeek = TicksPerDay * 7;

        private:
            Int64 _ticks;
    };
}

#endif
