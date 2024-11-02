
#include "Logger.h"

#include <codecvt>
#include <fstream>
#include <Shlobj.h>
#include "../Engine.h"
#include <cwchar>
Logger* Logger::instance;

Logger::Logger() {
    instance = this;
}

Logger::~Logger() = default;

bool Logger::write_log_to_file(const std::string &log) {
    std::wfstream logFile;

    const auto log_dir = LogDirectory();
    const auto log_file = LogFile();
    const auto path = log_dir + "\\" + log_file;

    logFile.open(path.c_str(), std::ios_base::app);

    if (!logFile.is_open()) {
        MessageBox(nullptr, "Unable to open log file...", "Log Error", MB_OK);
        return true;
    }

    logFile << log.c_str() << std::endl;
    logFile.close();
    return false;
}

void Logger::info(const std::string& log, ...) {
    CHAR outputBuffer[4096];

    va_list args;
    va_start(args, log);
    vsprintf_s(outputBuffer, log.c_str(), args);
    va_end(args);

    write_log_to_file("[" + Time::GetDateTimeString() + "]  " + outputBuffer);
}

void Logger::separator() {
    write_log_to_file("");
    write_log_to_file("----------------------------------------------------------------------------------------------------");
    write_log_to_file("");
}

std::string Logger::LogDirectory() {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;

    WCHAR* AppDataLocal;
    SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &AppDataLocal);

    const std::string baseDir = conv.to_bytes(AppDataLocal) + "\\" + PerGameSettings::GameName();
    const std::string logDir = baseDir + "\\Log";

    CreateDirectory(baseDir.c_str(), nullptr);
    CreateDirectory(logDir.c_str(), nullptr);
    return logDir;
}

std::string Logger::LogFile() {
    return PerGameSettings::GameName() + PerGameSettings::BootTime() + ".log";
}