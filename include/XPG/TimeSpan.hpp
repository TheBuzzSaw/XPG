#ifndef XpgTimeSpanHpp
#define XpgTimeSpanHpp

#include "Platform.hpp"
#include "DataTypes.hpp"

namespace XPG
{
    XpgClass TimeSpan
    {
        public:
            TimeSpan();
            explicit TimeSpan(Int64 ticks);
            TimeSpan(const TimeSpan& timeSpan);
            ~TimeSpan();

            inline Int64 Ticks() const { return _ticks; }

            TimeSpan& operator=(const TimeSpan& timeSpan);
            TimeSpan& operator+=(const TimeSpan& timeSpan);
            TimeSpan& operator-=(const TimeSpan& timeSpan);

            bool operator==(const TimeSpan& timeSpan) const;
            bool operator!=(const TimeSpan& timeSpan) const;
            bool operator<(const TimeSpan& timeSpan) const;
            bool operator<=(const TimeSpan& timeSpan) const;
            bool operator>(const TimeSpan& timeSpan) const;
            bool operator>=(const TimeSpan& timeSpan) const;

            const TimeSpan operator+(const TimeSpan& timeSpan) const;
            const TimeSpan operator-(const TimeSpan& timeSpan) const;
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

        private:
            Int64 _ticks;
    };
}

#endif
