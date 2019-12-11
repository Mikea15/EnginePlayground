# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project (EnginePlayground)

set(CMAKE_CXX_STANDARD 17)

include_directories(Engine)

# Include sub-projects.
add_subdirectory(Engine)
add_subdirectory(Launcher)

# 
add_dependencies(Launcher Engine)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Launcher)

