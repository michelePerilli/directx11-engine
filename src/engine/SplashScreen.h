#pragma once

#include "../platform/win32/Window.h"

namespace SplashScreen {



    VOID __declspec(dllexport) Open();
    VOID __declspec(dllexport) Close();
    VOID __declspec(dllexport) AddMessage(CONST WCHAR* message);

}

class __declspec(dllexport) SplashWindow : public Win32::Window {

public:
    SplashWindow();
    ~SplashWindow();

    virtual			LRESULT				MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;

private:
    WCHAR m_outputMessage[MAX_NAME_STRING];
};