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
        Time::Timer timer;

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

            if (timer.Peek() >= 1.0f) {
                EntryPointApplication->SetFPS(frameCount);
                frameCount = 0;
                timer.Mark();
            }
        }
    } catch (EngineException &e) {
        Logger::info("EngineException - %s",  e.what());
    } catch (std::exception &e) {
        Logger::info("std::exception - %s", e.what());
    }
    return 0;
}
