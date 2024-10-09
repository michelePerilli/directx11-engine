
#include "Logger.h"
#include <fstream>
#include <Shlobj.h>
#include "../Engine.h"

Logger* Logger::instance;

Logger::Logger() {
    instance = this;
}

Logger::~Logger() = default;

void Logger::info(const std::string& log) {

    std::wfstream logFile;

    const auto log_dir = LogDirectory();
    const auto log_file = LogFile();
    const auto path = std::wstring(log_dir + L"\\" + log_file);

    logFile.open(path.c_str(), std::ios_base::app);

    if (!logFile.is_open()) {
        MessageBox(nullptr, "Unable to open log file...", "Log Error", MB_OK);
        return;
    }

    logFile << L"[" << Time::GetDateTimeString() << L"]  " << log.c_str() << std::endl;
    logFile.close();

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