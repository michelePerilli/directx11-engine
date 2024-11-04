cmake_minimum_required(VERSION 3.29)
project(GameEngine)

set(CMAKE_CXX_STANDARD 26)

add_definitions(-DBUILD_DLL)


file(GLOB_RECURSE srcs src/*.cpp src/*.h)
add_library(GameEngine SHARED ${srcs})

target_include_directories(GameEngine PUBLIC ${PROJECT_SOURCE_DIR}/include)

