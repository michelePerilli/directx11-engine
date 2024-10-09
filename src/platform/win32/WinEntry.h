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
    return 0;
}