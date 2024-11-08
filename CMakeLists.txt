cmake_minimum_required(VERSION 3.29)
project(GameEngine)

set(CMAKE_CXX_STANDARD 26)

add_definitions(-DBUILD_DLL)

# Build HLSL shaders
add_custom_target(shader)
set(vertex shader/ColorBlendVS.hlsl shader/ColorIndexVS.hlsl shader/TextureVS.hlsl)
set(pixel shader/ColorBlendPS.hlsl shader/ColorIndexPS.hlsl shader/TexturePS.hlsl)

set(HLSL_SHADER_FILES ${vertex} ${pixel})

set_source_files_properties(${vertex} PROPERTIES ShaderType "vs")
set_source_files_properties(${pixel} PROPERTIES ShaderType "ps")
set_source_files_properties(${HLSL_SHADER_FILES} PROPERTIES ShaderModel "4_0")

foreach(FILE ${HLSL_SHADER_FILES})
    get_filename_component(FILE_WE ${FILE} NAME_WE)
    get_source_file_property(shadertype ${FILE} ShaderType)
    get_source_file_property(shadermodel ${FILE} ShaderModel)
    add_custom_command(TARGET shader
            COMMAND fxc.exe /nologo /Emain /T${shadertype}_${shadermodel} $<IF:$<CONFIG:DEBUG>,/Od,/O1> /Zi /Fo ${CMAKE_CURRENT_SOURCE_DIR}/shader/${FILE_WE}.cso #[[/Fd ${CMAKE_CURRENT_SOURCE_DIR}/${FILE_WE}.pdb]] ${FILE}
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