#pragma once

#include <chrono>
#include <minwindef.h>
#include <string>
#include <iomanip>

#include "../core/CoreMacro.h"

namespace Time {
    /* Get current time in string format */
    std::string ENGINE_DLL GetTime(BOOL striped = false);

    /* Get current date in string format */
    std::string ENGINE_DLL GetDate(BOOL striped = false);

    /* Get current date and time in string format */
    std::string ENGINE_DLL GetDateTimeString(BOOL striped = false);

    class ENGINE_DLL Timer {
    public:
        Timer() noexcept;

        float Mark() noexcept;

        [[nodiscard]] float Peek() const noexcept;

    private:
        std::chrono::steady_clock::time_point last;
    };
}
