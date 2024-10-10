#pragma once
#include <string>
#include <Windows.h>

class __declspec(dllexport) Logger {
    static Logger *instance;

public:
    static Logger *getInstance() { return instance; }

    Logger();

    ~Logger();

    static bool write_log_to_file(const std::string &log);

    static VOID info(const std::string &log);

    static void separator();

    static std::wstring LogDirectory();

    static std::wstring LogFile();
};
