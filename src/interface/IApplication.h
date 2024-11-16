#pragma once
#include "../core/WinSDK.h"
#include "../core/CoreMacro.h"

#define ENTRYAPP(x) Win32::IApplication* EntryApplication() { return new x; }

namespace Win32 {
    class ENGINE_DLL IApplication {
    public:
        IApplication() = default;

        virtual ~IApplication() = default;

        virtual VOID SetupPerGameSettings() = 0;

        virtual VOID PreInitialize() = 0;

        virtual VOID Initialize() = 0;

        virtual VOID Update() = 0;

        virtual VOID SetFPS(int fps) = 0;
    };

    IApplication *EntryApplication();
}
