#pragma once
#include <libloaderapi.h>
#include <string>
#include <windows.h>

class ENGINE_DLL PerGameSettings {

    static PerGameSettings* inst;

    static PerGameSettings* Instance() { return inst; }

public:

    PerGameSettings();
    ~PerGameSettings();

private:

    int exit_button = -1;
    std::string m_GameName;
    std::string m_ShortName;
    HICON m_MainIcon{};
    std::string m_BootTime;
    std::string m_SplashURL;

public:
    [[nodiscard]] static int ExitButton() {
        return inst->exit_button;
    }

    static void SetExitButton(const int exitButton) {
        inst->exit_button = exitButton;
    }

    static std::string GameName() { return inst->m_GameName; }

    static VOID SetGameName(const std::string &name) {
        inst->m_GameName = name;
    }

    static std::string ShortName() { return inst->m_ShortName; }

    static VOID SetShortName(const std::string &shortName) {
        inst->m_ShortName = shortName;
    }

    static HICON MainIcon() { return inst->m_MainIcon; }

    static VOID SetMainIcon(UINT id) {

        LoadIcon(HInstance(), MAKEINTRESOURCE(id));
    }

    static std::string BootTime() { return inst->m_BootTime; }

    static std::string SplashURL() { return inst->m_SplashURL; }

    // static VOID SetSplashURL(UINT id) { LoadString(HInstance(), id, inst->m_SplashURL, MAX_NAME_STRING); }
};
