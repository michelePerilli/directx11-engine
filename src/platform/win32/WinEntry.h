#pragma once
#include "../../Engine.h"
#include "../../common/Logger.h"

extern IApplication* EntryApplication();


int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    const auto EntryPointApplication = EntryApplication();

    PerGameSettings GameSettings;
    EntryPointApplication->SetupPerGameSettings();

    Logger logger;

    EntryPointApplication->Initialize();


    MSG msg = { 0 };
    while (msg.message != WM_QUIT)
    {
        // If there are Window messages then process them.
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            EntryPointApplication->Update();
        }
    }

    return 0;
}