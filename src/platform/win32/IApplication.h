#pragma once

#define ENTRYAPP(x) Win32::IApplication* EntryApplication() { return new x; }

namespace Win32 {
    class __declspec(dllexport) IApplication {
    public:
        IApplication();

        virtual ~IApplication() = default;

        virtual VOID SetupPerGameSettings() = 0;

        virtual VOID PreInitialize() = 0;

        virtual VOID Initialize() = 0;

        virtual VOID Update() = 0;
    };

    IApplication* EntryApplication();
}
