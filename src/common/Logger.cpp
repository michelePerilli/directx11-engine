
#include "Logger.h"
#include <fstream>
#include <Shlobj.h>
#include "../Engine.h"

Logger* Logger::instance;

Logger::Logger() {
    instance = this;
}

Logger::~Logger() = default;

bool Logger::write_log_to_file(const std::string &log) {
    std::wfstream logFile;

    const auto log_dir = LogDirectory();
    const auto log_file = LogFile();
    const auto path = std::wstring(log_dir + L"\\" + log_file);

    logFile.open(path.c_str(), std::ios_base::app);

    if (!logFile.is_open()) {
        MessageBox(nullptr, "Unable to open log file...", "Log Error", MB_OK);
        return true;
    }

    logFile << log.c_str() << std::endl;
    logFile.close();
    return false;
}

void Logger::info(const std::string& log) {
    std::stringstream ss;
    ss << "[" << Time::GetDateTimeString() << "]  " << log;
    Logger::write_log_to_file(ss.str());
}

void Logger::separator() {
    Logger::write_log_to_file("");
    Logger::write_log_to_file("----------------------------------------------------------------------------------------------------");
    Logger::write_log_to_file("");
}

std::wstring Logger::LogDirectory()
{
    WCHAR Path[1024];
    WCHAR* AppDataLocal;
    SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &AppDataLocal);
    wcscpy_s(Path, AppDataLocal);
    wcscat_s(Path, L"\\");
    wcscat_s(Path, PerGameSettings::GameName().c_str());
    CreateDirectoryW(Path, nullptr);
    wcscat_s(Path, L"\\Log");
    CreateDirectoryW(Path, nullptr);
    return Path;
}

std::wstring Logger::LogFile()
{
    WCHAR File[1024];
    wcscpy_s(File, PerGameSettings::GameName().c_str());
    wcscat_s(File, PerGameSettings::BootTime().c_str());
    wcscat_s(File, L".log");
    return File;
}