#pragma once

#include <windows.h>

// #ifdef BUILD_DLL
//     #define ENGINE_API __declspec(dllexport)
// #else
//     #define ENGINE_API __declspec(dllimport)
// #endif

#define MAX_NAME_STRING 256
#define HInstance() GetModuleHandle(NULL)

#include "Common/Logger.h"
#include "Common/Time.h"
#include "core/PerGameSettings.h"