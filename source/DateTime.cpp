#include "../include/XPG/DateTime.hpp"
#include "TickSpans.hpp"
#include <ctime>

namespace XPG
{
    static const int DaysInMonths[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30,
        31, 30, 31 };

    static const Int64 DaysPerYear = 365;
    static const Int64 TicksPerYear = TicksPerDay * DaysPerYear;
    static const Int64 DaysPerFourCenturies = DaysPerYear * 400 + 97;
    static const Int64 DaysPerCentury = DaysPerYear * 100 + 24;
    static const Int64 DaysPerFourYears = DaysPerYear * 4 + 1;

    DateTime::DateTime() : _ticks(0)
    {
    }

    DateTime::DateTime(Int64 ticks) : _ticks(ticks)
    {
        Validate();
    }

    DateTime::DateTime(int year, int month, int day, int hour, int minute,
        int second, int millisecond, int microsecond) : _ticks(0)
    {
        Set(year, month, day, hour, minute, second, millisecond, microsecond);
    }

    DateTime::DateTime(const DateTime& dateTime) : _ticks(dateTime._ticks)
    {
    }

    DateTime::~DateTime()
    {
    }

    void DateTime::Validate()
    {
        if (_ticks < 0) _ticks = 0;
    }

    Int64 DateTime::ExtractYears(Int64& days) const
    {
        Int64 year = 1;

        if (days >= DaysPerFourCenturies)
        {
            Int64 chunks = days / DaysPerFourCenturies;
            year += chunks * 400;
            days -= chunks * DaysPerFourCenturies;
        }

        if (days >= DaysPerCentury)
        {
            Int64 chunks = days / DaysPerCentury;
            if (chunks == 4) chunks = 3;
            year += chunks * 100;
            days -= chunks * DaysPerCentury;
        }

        if (days >= DaysPerFourYears)
        {
            Int64 chunks = days / DaysPerFourYears;
            year += chunks * 4;
            days -= chunks * DaysPerFourYears;
        }

        if (days >= DaysPerYear)
        {
            Int64 chunks = days / DaysPerYear;
            if (chunks == 4) chunks = 3;
            year += chunks;
            days -= chunks * DaysPerYear;
        }

        return year;
    }

    Int64 DateTime::ExtractMonth(Int64& days, int year) const
    {
        Int64 month = 1;

        for (Int64 daysInMonth = DaysInMonth(month, year);
            days > daysInMonth; daysInMonth = DaysInMonth(month, year))
        {
            ++month;
            days -= daysInMonth;
        }

        return month;
    }

    const TimeSpan DateTime::TimeSinceMidnight() const
    {
        return TimeSpan(_ticks % TicksPerDay);
    }

    const DateTime DateTime::DateOnly() const
    {
        Int64 days = _ticks / TicksPerDay;
        return DateTime(days * TicksPerDay);
    }

    bool DateTime::Set(int year, int month, int day, int hour, int minute,
        int second, int millisecond, int microsecond)
    {
        bool isValidDate = false;

        if (year >= 1 && year <= 9999
            && day >= 1 && day <= DaysInMonth(month, year)
            && hour >= 0 && hour <= 23
            && minute >= 0 && minute <= 59
            && second >= 0 && second <= 59
            && millisecond >= 0 && millisecond <= 999
            && microsecond >= 0 && microsecond <= 999)
        {
            Int64 days = day - 1;

            for (int i = 1; i < month; ++i)
                days += DaysInMonth(i, year);

            --year;
            days += (year * DaysPerYear) + (year / 4) - (year / 100)
                + (year / 400);

            _ticks = days * TicksPerDay;
            _ticks += hour * TicksPerHour;
            _ticks += minute * TicksPerMinute;
            _ticks += second * TicksPerSecond;
            _ticks += millisecond * TicksPerMillisecond;
            _ticks += microsecond * TicksPerMicrosecond;

            isValidDate = true;
        }

        return isValidDate;
    }

    int DateTime::DayOfWeek() const
    {
        return (_ticks / TicksPerDay) % 7;
    }

    int DateTime::Year() const
    {
        Int64 days = _ticks / TicksPerDay;
        return ExtractYears(days);
    }

    int DateTime::Month() const
    {
        Int64 days = _ticks / TicksPerDay;
        Int64 year = ExtractYears(days);
        return ExtractMonth(days, year);
    }

    int DateTime::Day() const
    {
        Int64 days = _ticks / TicksPerDay;
        Int64 year = ExtractYears(days);
        ExtractMonth(days, year);
        return days + 1;
    }

    int DateTime::Hour() const
    {
        Int64 hours = _ticks / TicksPerHour;
        return hours % 24;
    }

    int DateTime::Minute() const
    {
        Int64 minutes = _ticks / TicksPerMinute;
        return minutes % 60;
    }

    int DateTime::Second() const
    {
        Int64 seconds = _ticks / TicksPerSecond;
        return seconds % 60;
    }

    int DateTime::Millisecond() const
    {
        Int64 milliseconds = _ticks / TicksPerMillisecond;
        return milliseconds % 1000;
    }

    int DateTime::Microsecond() const
    {
        Int64 microseconds = _ticks / TicksPerMicrosecond;
        return microseconds % 1000;
    }

    DateTime& DateTime::operator=(const DateTime& dateTime)
    {
        _ticks = dateTime._ticks;
        return *this;
    }

    DateTime& DateTime::operator+=(const TimeSpan& timeSpan)
    {
        _ticks += timeSpan.Ticks();
        Validate();
        return *this;
    }

    DateTime& DateTime::operator-=(const TimeSpan& timeSpan)
    {
        _ticks -= timeSpan.Ticks();
        Validate();
        return *this;
    }

    bool DateTime::operator==(const DateTime& dateTime) const
    {
        return _ticks == dateTime._ticks;
    }

    bool DateTime::operator!=(const DateTime& dateTime) const
    {
        return _ticks != dateTime._ticks;
    }

    bool DateTime::operator<(const DateTime& dateTime) const
    {
        return _ticks < dateTime._ticks;
    }

    bool DateTime::operator<=(const DateTime& dateTime) const
    {
        return _ticks <= dateTime._ticks;
    }

    bool DateTime::operator>(const DateTime& dateTime) const
    {
        return _ticks > dateTime._ticks;
    }

    bool DateTime::operator>=(const DateTime& dateTime) const
    {
        return _ticks >= dateTime._ticks;
    }

    const DateTime DateTime::operator+(const TimeSpan& timeSpan) const
    {
        return DateTime(_ticks + timeSpan.Ticks());
    }

    const DateTime DateTime::operator-(const TimeSpan& timeSpan) const
    {
        return DateTime(_ticks - timeSpan.Ticks());
    }

    const TimeSpan DateTime::operator-(const DateTime& dateTime) const
    {
        return TimeSpan(_ticks - dateTime._ticks);
    }

    int DateTime::DaysInMonth(int month, int year)
    {
        int days = 0;

        if (month >= 1 && month <= 12)
        {
            days = DaysInMonths[month];

            if (month == 2 && IsLeapYear(year))
                days = 29;
        }

        return days;
    }

    int DateTime::DaysInYear(int year)
    {
        return IsLeapYear(year) ? 366 : 365;
    }

    bool DateTime::IsLeapYear(int year)
    {
        if (year % 4)
            return false;

        if (year % 100)
            return true;

        if (year % 400)
            return false;

        return true;
    }

    const char* DateTime::DayToString(int dayOfWeek)
    {
        const char* dayName = "invalid day";

        switch (dayOfWeek)
        {
            case 0:
                dayName = "Monday";
                break;

            case 1:
                dayName = "Tuesday";
                break;

            case 2:
                dayName = "Wednesday";
                break;

            case 3:
                dayName = "Thursday";
                break;

            case 4:
                dayName = "Friday";
                break;

            case 5:
                dayName = "Saturday";
                break;

            case 6:
                dayName = "Sunday";
                break;

            default:
                break;
        }

        return dayName;
    }

    static const DateTime GetDateTime(tm* timeInfo)
    {
        int second = timeInfo->tm_sec;

        // http://www.cplusplus.com/reference/clibrary/ctime/tm/
        if (second > 59)
            second = 59;

        return DateTime(timeInfo->tm_year + 1900, timeInfo->tm_mon + 1,
            timeInfo->tm_mday, timeInfo->tm_hour, timeInfo->tm_min, second);
    }

    const DateTime DateTime::LocalTime()
    {
        time_t rawTime;
        time(&rawTime);
        tm* timeInfo = localtime(&rawTime);

        return GetDateTime(timeInfo);
    }

    const DateTime DateTime::UtcTime()
    {
        time_t rawTime;
        time(&rawTime);
        tm* timeInfo = gmtime(&rawTime);

        return GetDateTime(timeInfo);
    }

}
