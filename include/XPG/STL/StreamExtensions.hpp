#ifndef XpgStreamExtensionsHpp
#define XpgStreamExtensionsHpp

#include "../DateTime.hpp"
#include <iostream>

namespace XPG
{
    template<typename TraitsT>
    std::basic_ostream<char, TraitsT>& operator<<(
        std::basic_ostream<char, TraitsT>& stream, const DateTime& dateTime)
    {
        stream << dateTime.Year() << '-' << dateTime.Month() << '-'
            << dateTime.Day() << ' ';

        stream << dateTime.Hour() << ':';

        int minute = dateTime.Minute();
        if (minute < 10) stream << '0';
        stream << minute << ':';

        int second = dateTime.Second();
        if (second < 10) stream << '0';
        stream << second;

        return stream;
    }

    template<typename TraitsT>
    std::basic_ostream<char, TraitsT>& operator<<(
        std::basic_ostream<char, TraitsT>& stream, const TimeSpan& timeSpan)
    {
        Int64 count = timeSpan.ToWeeks();
        stream << count << 'w';

        count = timeSpan.ToDays() - timeSpan.ToWeeks() * 7;
        stream << count << 'd';

        count = timeSpan.ToHours() - timeSpan.ToDays() * 24;
        stream << count << 'h';

        count = timeSpan.ToMinutes() - timeSpan.ToHours() * 60;
        stream << count << 'm';

        count = timeSpan.ToSeconds() - timeSpan.ToMinutes() * 60;
        stream << count << 's';

        count = timeSpan.ToMilliseconds() - timeSpan.ToSeconds() * 1000;
        stream << count << "ms";

        count = timeSpan.ToMicroseconds() - timeSpan.ToMilliseconds() * 1000;
        stream << count << "us";

        return stream;
    }
}

#endif
