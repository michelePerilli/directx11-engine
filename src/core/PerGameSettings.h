#pragma once
#include <libloaderapi.h>
#include <string>
#include <windows.h>

class __declspec(dllexport) PerGameSettings {

    static PerGameSettings* inst;

    static PerGameSettings* Instance() { return inst; }

public:

    PerGameSettings();
    ~PerGameSettings();

private:

    std::wstring m_GameName;
    std::wstring m_ShortName;
    // HICON m_MainIcon{};
    std::wstring m_BootTime;
    std::wstring m_SplashURL;

public:

    static std::wstring GameName() { return inst->m_GameName; }

    static VOID SetGameName(const std::wstring &name) {
        inst->m_GameName = name;
    }

    static std::wstring ShortName() { return inst->m_ShortName; }

    static VOID SetShortName(const std::wstring &shortName) {
        inst->m_ShortName = shortName;
    }

    // static HICON MainIcon() { return inst->m_MainIcon; }

    static VOID SetMainIcon(UINT id) {
        LoadIcon(HInstance(), MAKEINTRESOURCE(id));
    }

    static std::wstring BootTime() { return inst->m_BootTime; }

    static std::wstring SplashURL() { return inst->m_SplashURL; }
    // static VOID SetSplashURL(UINT id) { LoadString(HInstance(), id, inst->m_SplashURL, MAX_NAME_STRING); }
};
