cmake_minimum_required(VERSION 3.29)
project(GameEngine)

set(CMAKE_COMPILE_WARNING_AS_ERROR OFF)
set(CMAKE_CXX_STANDARD 26)

add_definitions(-DBUILD_DLL)

file(GLOB_RECURSE vertex_files shader/*VS*.hlsl)
file(GLOB_RECURSE pixel_files shader/*PS*.hlsl)

# Build HLSL shaders
add_custom_target(shader)
set(vertex "")
set(pixel "")
foreach (vf ${vertex_files})
    string(APPEND vertex ${vf} ";")
endforeach ()
foreach (pf ${pixel_files})
    set(pixel ${pixel} ${pf} ";")
endforeach ()

set(HLSL_SHADER_FILES ${vertex} ${pixel})

set_source_files_properties(${vertex} PROPERTIES ShaderType "vs")
set_source_files_properties(${pixel} PROPERTIES ShaderType "ps")
set_source_files_properties(${HLSL_SHADER_FILES} PROPERTIES ShaderModel "5_0")

foreach(FILE ${HLSL_SHADER_FILES})
    get_filename_component(FILE_WE ${FILE} NAME_WE)
    get_source_file_property(shadertype ${FILE} ShaderType)
    get_source_file_property(shadermodel ${FILE} ShaderModel)
    add_custom_command(TARGET shader
            COMMAND fxc.exe /nologo /Emain /T${shadertype}_${shadermodel} $<IF:$<CONFIG:DEBUG>,/Od,/O1> /Zi /Fo ${CMAKE_CURRENT_SOURCE_DIR}/shader/_${FILE_WE}.cso #[[/Fd ${CMAKE_CURRENT_SOURCE_DIR}/${FILE_WE}.pdb]] ${FILE}
            MAIN_DEPENDENCY ${FILE}
            COMMENT "HLSL ${FILE}"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            VERBATIM)
endforeach(FILE)


file(GLOB_RECURSE srcs src/*.cpp src/*.h)
add_library(GameEngine SHARED ${srcs})

add_dependencies(GameEngine shader)

target_include_directories(GameEngine PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_subdirectory(libs/DirectXMath-main)
target_link_libraries(GameEngine d3d11)
target_link_libraries(GameEngine d3dx11)
target_link_libraries(GameEngine D3DCompiler.lib)
target_link_libraries(GameEngine DirectXMath)

include(FetchContent)
include(CMakeLists-assimp.txt)