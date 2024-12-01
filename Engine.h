#pragma once

#ifdef WIN_ENTRY
#include "src/WinEntry.h"
#endif

#include "src/component/imgui/imgui_impl_dx11.h"
#include "src/component/imgui/imgui_impl_win32.h"
#include "src/component/gdiplus/GDIPlusManager.h"
#include "src/component/ImguiManager.h"
#include "src/component/Simulation.h"
#include "src/component/pipeline/Drawable.h"
#include "src/component/pipeline/Bindable.h"
#include "src/interface/Keyboard.h"
#include "src/interface/Mouse.h"
#include "src/interface/IApplication.h"
#include "src/interface/Graphics.h"
#include "src/interface/Panel.h"
#include "src/interface/Camera.h"
#include "src/model/PointLight.h"
#include "src/model/Mesh.h"
#include "src/model/Sampler.h"
#include "src/core/WinSDK.h"
#include "src/core/CoreMacro.h"
#include "src/component/Time.h"
#include "src/component/Logger.h"
#include "src/core/PerGameSettings.h"
