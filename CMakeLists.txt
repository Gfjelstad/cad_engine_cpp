cmake_minimum_required(VERSION 3.16)
project(nodus VERSION 1.0.0 LANGUAGES CXX)

# ==============================================================================
# Project Configuration
# ==============================================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()

# ==============================================================================
# Third-party Dependencies
# ==============================================================================

# OpenCASCADE Configuration
set(OpenCASCADE_DIR "${CMAKE_SOURCE_DIR}/external/opencascade/occ/cmake")
find_package(OpenCASCADE REQUIRED)

# nlohmann/json
include(FetchContent)
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)


# ==============================================================================
# OpenCASCADE Interface Library
# ==============================================================================
add_library(opencascade_wrapper INTERFACE)

target_include_directories(opencascade_wrapper INTERFACE
    ${OpenCASCADE_INCLUDE_DIR}
    "${CMAKE_SOURCE_DIR}/external/opencascade/occ/inc"
)

# Link directories based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_directories(opencascade_wrapper INTERFACE 
        "${CMAKE_SOURCE_DIR}/external/opencascade/occ/win64/vc14/libd"
    )
else()
    target_link_directories(opencascade_wrapper INTERFACE 
        "${CMAKE_SOURCE_DIR}/external/opencascade/occ/win64/vc14/lib"
    )
endif()

target_link_libraries(opencascade_wrapper INTERFACE
    TKMath 
    TKernel 
    TKGeomBase 
    TKTopAlgo
)

add_subdirectory(nodusComLib)
add_subdirectory(nodus)

enable_testing()
add_subdirectory(tests)