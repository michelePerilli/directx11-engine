#pragma once
#include "../platform/win32/IApplication.h"

namespace Engine {

    class __declspec(dllexport) Simulation : public Win32::IApplication {
    public:
        Simulation();

        ~Simulation() override;

        virtual VOID PreInitialize() override;
    };
}
