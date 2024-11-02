#pragma once

class ENGINE_DLL OEngine;

namespace Engine {
    enum EngineMode : INT {
        NONE,
        DEBUG,
        RELEASE,
        EDITOR,
        SERVER
    };

    extern OEngine gEngine;

    VOID ENGINE_DLL SetMode(EngineMode mode);
    EngineMode ENGINE_DLL GetMode();

    std::string ENGINE_DLL EngineModeToString();

}

using namespace Engine;

class ENGINE_DLL OEngine {
private:
    EngineMode engineMode;

public:
    OEngine();

    ~OEngine();

    EngineMode GetEngineMode() const;
    VOID SetEngineMode(EngineMode mode);
};
