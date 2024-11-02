#pragma once

#include <minwindef.h>
#include <string>
#include <iomanip>

#include "../core/CoreDef.h"

namespace Time {

    /* Get current time in string format */
    std::string ENGINE_DLL GetTime(BOOL striped = false);

    /* Get current date in string format */
    std::string ENGINE_DLL GetDate(BOOL striped = false);

    /* Get current date and time in string format */
    std::string ENGINE_DLL GetDateTimeString(BOOL striped = false);
}