#pragma once
#include "interface/IApplication.h"
#include "component/Logger.h"
#include "component/Time.h"
#include "component/gdiplus/GDIPlusManager.h"
#include "core/PerGameSettings.h"

extern Win32::IApplication *EntryApplication();


int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    try {

        Logger logger;
        PerGameSettings gameSettings;
        Logger::info("Engine initialization...");
        const auto EntryPointApplication = EntryApplication();

        Logger::info("Setting up things...");
        EntryPointApplication->SetupPerGameSettings();

        Logger::info("Loading application...");
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
    } catch (Exception &e) {
        Logger::error(e);
    } catch (std::exception &e) {
        Logger::info("std::exception - %s", e.what());
    }
    return 0;
}
