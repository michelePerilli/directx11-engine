#pragma once

#include "../platform/win32/Window.h"

namespace SplashScreen {

    VOID ENGINE_DLL Open();
    VOID ENGINE_DLL Close();
    VOID ENGINE_DLL AddMessage(const std::string& message);

}

class ENGINE_DLL SplashWindow final : public Win32::Window {

public:
    SplashWindow();

    ~SplashWindow() override;

    LRESULT MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;

private:
    std::string m_outputMessage;

};
