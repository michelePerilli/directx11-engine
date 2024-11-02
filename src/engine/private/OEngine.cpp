#include "../../Engine.h"

namespace Engine {
    OEngine gEngine;
    VOID SetMode(const EngineMode mode) {
        gEngine.SetEngineMode(mode);
    }

    EngineMode GetMode() {
        return gEngine.GetEngineMode();
    }

    std::string ENGINE_DLL EngineModeToString() {
        switch (GetMode()) {
            case DEBUG:
                return "Debug";
            case RELEASE:
                return "Release";
            case SERVER:
                return "Server";
            case EDITOR:
                return "Editor";
            default:
                return "None";
        }
    }
}

OEngine::OEngine() {
#ifdef _DEBUG
        engineMode = DEBUG;
#else
    engineMode = RELEASE;
    // engineMode = DEBUG;
#endif
}

OEngine::~OEngine() = default;

EngineMode OEngine::GetEngineMode() const {
    return engineMode;
}

VOID OEngine::SetEngineMode(const EngineMode mode) {
    engineMode = mode;
}