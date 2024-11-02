#pragma once
#include <string>
#include <Windows.h>

#include "../core/CoreDef.h"

class ENGINE_DLL Logger {
    static Logger *instance;

public:
    static Logger *getInstance() { return instance; }

    Logger();

    ~Logger();

    static bool write_log_to_file(const std::string &log);

    static VOID info(const std::string &log, ...);

    static void separator();

    static std::string LogDirectory();

    static std::string LogFile();
};
