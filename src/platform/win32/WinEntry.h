#pragma once
#include <chrono>

#include "IApplication.h"
#include "Panel.h"
#include "../../interface/Keyboard.h"
#include "../../common/Logger.h"
#include "../../exception/EngineException.h"

extern Win32::IApplication *EntryApplication();


int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    try {
        const auto EntryPointApplication = EntryApplication();

        PerGameSettings GameSettings;
        EntryPointApplication->SetupPerGameSettings();

        Logger logger;

        EntryPointApplication->PreInitialize();

        EntryPointApplication->Initialize();
        int frameCount = 0;
        auto lastTime = std::chrono::high_resolution_clock::now();
        Panel wnd("Pippo", 800, 200);

        MSG msg = {nullptr};
        while (msg.message != WM_QUIT) {
            // If there are Window messages then process them.
            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            } else {

                if (wnd.keyboard.KeyIsPressed( VK_SPACE) ) {
                    Logger::info("KeyPressed Space");
                }
                while (!wnd.mouse.IsEmpty()) {
                    if (wnd.mouse.Read().GetType() == Mouse::Event::Type::Move) {
                        std::ostringstream s;
                        s << "Mouse: " << wnd.mouse.GetPosX() << ", " << wnd.mouse.GetPosY();
                        wnd.SetTitle(s.str());
                    }
                }
                EntryPointApplication->Update();
            }

            frameCount++;

            auto currentTime = std::chrono::high_resolution_clock::now();

            if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count() / 1000.0f >=
                1.0f) {
                // std::cout << "FPS: " << frameCount << std::endl;
                // Logger::info("FPS: %d", frameCount);
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
