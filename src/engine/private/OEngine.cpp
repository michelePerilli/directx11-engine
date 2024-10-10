#include "../../Engine.h"

namespace Engine {
    OEngine gEngine;
    VOID SetMode(EngineMode mode)
    {
        gEngine.SetEngineMode(mode);
    }


    EngineMode GetMode()
    {
        return gEngine.GetEngineMode();
    }

    std::wstring __declspec(dllexport) EngineModeToString()
    {
        switch (Engine::GetMode())
        {
            case EngineMode::DEBUG:		return L"Debug";
            case EngineMode::RELEASE:	return L"Release";
            case EngineMode::SERVER:	return L"Server";
            case EngineMode::EDITOR:	return L"Editor";
            default:     return L"None";
        }
    }


}
OEngine::OEngine() {
#ifdef _DEBUG
        engineMode = DEBUG;
#else
    // engineMode = RELEASE;
    engineMode = DEBUG;
#endif
}

OEngine::~OEngine() {
}

EngineMode OEngine::GetEngineMode()
{
    return engineMode;
}

VOID OEngine::SetEngineMode(EngineMode mode)
{
    engineMode = mode;
}