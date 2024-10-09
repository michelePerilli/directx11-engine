#pragma once

#define ENTRYAPP(x) IApplication* EntryApplication() { return new x; }

class __declspec(dllexport) IApplication {
public:
    IApplication();
    virtual ~IApplication() = default;

    virtual VOID SetupPerGameSettings() = 0;
    virtual VOID Initialize() = 0;
    virtual VOID Update() = 0;
};

IApplication* EntryApplication();