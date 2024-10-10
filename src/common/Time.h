#pragma once

#include <minwindef.h>
#include <string>
#include <iomanip>

namespace Time {

    /* Get current time in string format */
    std::string __declspec(dllexport) GetTime(BOOL striped = false);

    /* Get current date in string format */
    std::string __declspec(dllexport) GetDate(BOOL striped = false);

    /* Get current date and time in string format */
    std::string __declspec(dllexport) GetDateTimeString(BOOL striped = false);
}