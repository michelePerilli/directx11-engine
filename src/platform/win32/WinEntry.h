#pragma once
#include "IApplication.h"
#include "../../common/Logger.h"

extern Win32::IApplication* EntryApplication();


int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    const auto EntryPointApplication = EntryApplication();

    PerGameSettings GameSettings;
    EntryPointApplication->SetupPerGameSettings();

    Logger logger;

    EntryPointApplication->PreInitialize();

    EntryPointApplication->Initialize();


    MSG msg = { nullptr };
    while (msg.message != WM_QUIT)
    {
        // If there are Window messages then process them.
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            EntryPointApplication->Update();
        }
    }

    return 0;
}