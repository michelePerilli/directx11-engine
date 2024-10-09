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
        src/core/PerGameSettings.cpp
        src/core/PerGameSettings.h
        src/common/Time.cpp
        src/common/Time.h)