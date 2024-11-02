cmake_minimum_required(VERSION 3.29)
project(GameEngine)

set(CMAKE_CXX_STANDARD 26)

file(GLOB_RECURSE srcs src/*.cpp src/*.h)
add_definitions(-DBUILD_DLL)
add_library(GameEngine SHARED ${srcs})

