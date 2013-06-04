#ifndef XpgDateTimeHpp
#define XpgDateTimeHpp

#include "Platform.hpp"
#include "TimeSpan.hpp"

namespace XPG
{
    class XpgApi DateTime
    {
        public:
            DateTime();
            explicit DateTime(Int64 ticks);
            DateTime(int year, int month, int day, int hour = 0, int minute = 0,
                int second = 0, int millisecond = 0, int microsecond = 0);
            DateTime(const DateTime& other);
            ~DateTime();

            inline Int64 Ticks() const { return _ticks; }
            inline operator bool() const { return _ticks > 0; }

            DateTime& operator=(const DateTime& other);
            DateTime& operator+=(const TimeSpan& time);
            DateTime& operator-=(const TimeSpan& time);

            bool operator==(const DateTime& other) const;
            bool operator!=(const DateTime& other) const;
            bool operator<(const DateTime& other) const;
            bool operator<=(const DateTime& other) const;
            bool operator>(const DateTime& other) const;
            bool operator>=(const DateTime& other) const;

            const DateTime operator+(const TimeSpan& time) const;
            const DateTime operator-(const TimeSpan& time) const;
            const TimeSpan operator-(const DateTime& other) const;

            const TimeSpan TimeOfDay() const;
            const DateTime Date() const;
            bool Set(int year, int month, int day, int hour = 0,
                int minute = 0, int second = 0, int millisecond = 0,
                int microsecond = 0);

            int DayOfWeek() const;
            int Year() const;
            int Month() const;
            int Day() const;
            int Hour() const;
            int Minute() const;
            int Second() const;
            int Millisecond() const;
            int Microsecond() const;

            static int DaysInMonth(int month, int year = 1);
            static int DaysInYear(int year);
            static bool IsLeapYear(int year);
            static const char* DayToString(int dayOfWeek);
            static const DateTime LocalTime();
            static const DateTime UtcTime();

        private:
            void Validate();
            Int64 ExtractYears(Int64& days) const;
            Int64 ExtractMonth(Int64& days, int year) const;

            Int64 _ticks;
    };
}

#endif
