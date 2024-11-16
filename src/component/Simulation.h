#pragma once
#include "../interface/IApplication.h"
#include "../core/CoreMacro.h"

namespace Engine {

    class ENGINE_DLL Simulation : public Win32::IApplication {
    public:
        Simulation();

        ~Simulation() override;

        VOID PreInitialize() override;
    };
}
