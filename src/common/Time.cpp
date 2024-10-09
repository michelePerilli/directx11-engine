#include "Time.h"

/* Get Time in format '00:00:00' */
std::wstring Time::GetTime(const BOOL striped) {
    const time_t now = time(nullptr);
    tm ltm{};
    localtime_s(&ltm, &now);
    std::wstringstream wss;
    wss << std::put_time(&ltm, L"%H:%M:%S");

    std::wstring timeString = wss.str();

    if (striped) {
        std::erase_if(timeString, [](const wchar_t c) {
            return c == L':';
        });
    }

    return timeString;
}

/* Get date in format '00/00/00' */
std::wstring Time::GetDate(const BOOL striped) {
    const time_t now = time(nullptr);
    tm ltm{};
    localtime_s(&ltm, &now);
    std::wstringstream wss;
    wss << std::put_time(&ltm, L"%d/%m/%y");
    std::wstring timeString = wss.str();

    if (striped) {
        std::erase_if(timeString, [](const wchar_t c) {
            return c == '/';
        });
    }

    return timeString;
}

/* Get date time in format '00/00/00 00:00:00' */
std::wstring Time::GetDateTimeString(const BOOL striped) {
    std::wstring timeString = GetDate(striped) + GetTime(striped);
    return timeString;
}
