#pragma once
#include "../platform/win32/IApplication.h"

namespace Engine {

    class ENGINE_DLL Simulation : public Win32::IApplication {
    public:
        Simulation();

        ~Simulation() override;

        VOID PreInitialize() override;
    };
}
