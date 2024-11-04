#pragma once

#include "../platform/win32/Panel.h"

namespace SplashScreen {

    VOID ENGINE_DLL Open();
    VOID ENGINE_DLL Close();
    VOID ENGINE_DLL AddMessage(const std::string& message);

}

class ENGINE_DLL SplashWindow {

public:
    SplashWindow();


private:
    std::string m_outputMessage;

};
