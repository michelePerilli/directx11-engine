#pragma once
#include <chrono>

#include "IApplication.h"
#include "Panel.h"
#include "../../common/Logger.h"
#include "../../exception/EngineException.h"

extern Win32::IApplication *EntryApplication();


int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    try {
        const auto EntryPointApplication = EntryApplication();

        Logger logger;
        PerGameSettings GameSettings;

        EntryPointApplication->SetupPerGameSettings();
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

            if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count() / 1000.0f >=
                1.0f) {
                EntryPointApplication->SetFPS(frameCount);
                frameCount = 0;
                lastTime = currentTime;
            }
        }
    } catch (EngineException &e) {
        Logger::info("EngineException - %s",  e.what());
    } catch (std::exception &e) {
        Logger::info("std::exception - %s", e.what());
    }
    return 0;
}
