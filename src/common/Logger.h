#pragma once
#include <string>
#include <Windows.h>

class __declspec(dllexport) Logger {
    static Logger *instance;

public:
    static Logger *getInstance() { return instance; }

    Logger();

    ~Logger();

    static VOID info(const std::string &log);

    static std::wstring LogDirectory();

    static std::wstring LogFile();
};
