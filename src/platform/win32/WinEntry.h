#pragma once
#include <chrono>
#include <iostream>

#include "IApplication.h"
#include "../../common/Logger.h"

extern Win32::IApplication *EntryApplication();


int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    const auto EntryPointApplication = EntryApplication();

    PerGameSettings GameSettings;
    EntryPointApplication->SetupPerGameSettings();

    Logger logger;

    EntryPointApplication->PreInitialize();

    EntryPointApplication->Initialize();
    int frameCount = 0;
    auto lastTime = std::chrono::high_resolution_clock::now();


    MSG msg = {nullptr};
    while (msg.message != WM_QUIT) {
        // If there are Window messages then process them.
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            EntryPointApplication->Update();
        }

        frameCount++;

        auto currentTime = std::chrono::high_resolution_clock::now();

        if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count() / 1000.0f >= 1.0f) {
            std::cout << "FPS: " << frameCount << std::endl;
            Logger::info("FPS: %d", frameCount);
            frameCount = 0;
            lastTime = currentTime;
        }
    }

    return 0;
}
