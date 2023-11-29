cmake_minimum_required(VERSION 3.15)
project(csgo_client)


add_compile_options(-w)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories("C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Include")
file(GLOB_RECURSE SOURCES "dx9/*.cpp" "dx9/*.c" "imgui/*.cpp" "src/*.cpp" "sdk/*.cpp" "cheats/*.cpp")

find_library(D3D9_LIBRARY 
            NAMES d3d9.lib dxgi.lib d3dcompiler.lib
            PATHS "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x86")


add_library(${PROJECT_NAME} SHARED maindll.cpp ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${D3D9_LIBRARY})
