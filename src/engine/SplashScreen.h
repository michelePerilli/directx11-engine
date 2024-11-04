#pragma once

#include "../platform/win32/Panel.h"

namespace SplashScreen {

    VOID ENGINE_DLL Open();
    VOID ENGINE_DLL Close();
    VOID ENGINE_DLL AddMessage(const std::string& message);

}

class ENGINE_DLL SplashWindow: public Panel {

public:
    SplashWindow();

    ~SplashWindow() override;

    LRESULT MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;

private:
    std::string m_outputMessage;

};
