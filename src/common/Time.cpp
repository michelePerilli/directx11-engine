#include "Time.h"

#include <codecvt>

/* Get Time in format '00:00:00' */
std::string Time::GetTime(const BOOL striped) {
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
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

    return converter.to_bytes(timeString);
}

/* Get date in format '00/00/00' */
std::string Time::GetDate(const BOOL striped) {
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

    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

    return converter.to_bytes(timeString);
}

/* Get date time in format '00/00/00 00:00:00' */
std::string Time::GetDateTimeString(const BOOL striped) {
    std::string timeString = GetDate(striped) + GetTime(striped);
    return timeString;
}
