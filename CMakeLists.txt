cmake_minimum_required(VERSION 3.29)
project(GameEngine)

set(CMAKE_CXX_STANDARD 26)

add_library(GameEngine SHARED
        src/Engine.cpp
        src/Engine.h
        src/platform/win32/IApplication.cpp
        src/platform/win32/IApplication.h
        src/platform/win32/WinEntry.h
        src/common/Logger.cpp
        src/common/Logger.h
        src/core/private/PerGameSettings.cpp
        src/core/PerGameSettings.h
        src/common/Time.cpp
        src/common/Time.h
        src/engine/OEngine.h
        src/engine/Simulation.h
        src/engine/private/OEngine.cpp
        src/platform/win32/Window.cpp
        src/platform/win32/Window.h
        src/platform/win32/SubObject.cpp
        src/platform/win32/SubObject.h
        src/platform/win32/Win32Utils.cpp
        src/platform/win32/Win32Utils.h
        src/engine/private/Simulation.cpp
        src/engine/SplashScreen.cpp
        src/engine/SplashScreen.h)