#pragma once

class __declspec(dllexport) OEngine;

namespace Engine {
    enum EngineMode : INT {
        NONE,
        DEBUG,
        RELEASE,
        EDITOR,
        SERVER
    };

    extern OEngine gEngine;

    VOID __declspec(dllexport) SetMode(EngineMode mode);
    EngineMode __declspec(dllexport) GetMode();

    std::wstring __declspec(dllexport) EngineModeToString();

}

using namespace Engine;

class __declspec(dllexport) OEngine {
private:
    EngineMode engineMode;

public:
    OEngine();

    ~OEngine();

    EngineMode GetEngineMode();
    VOID SetEngineMode(EngineMode mode);
};
