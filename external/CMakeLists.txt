# ==============================================================================
# Third-party Dependencies
# ==============================================================================

# OpenCASCADE Configuration
set(OpenCASCADE_DIR "${CMAKE_SOURCE_DIR}/external/opencascade/occt_vc14-64-with-debug/cmake")
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
    "${CMAKE_SOURCE_DIR}/external/opencascade/occt_vc14-64-with-debug/inc"
)

# Link directories based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_directories(opencascade_wrapper INTERFACE 
        "${CMAKE_SOURCE_DIR}/external/opencascade/occt_vc14-64-with-debug/win64/vc14/libd"
    )
else()
    target_link_directories(opencascade_wrapper INTERFACE 
        "${CMAKE_SOURCE_DIR}/external/opencascade/occt_vc14-64-with-debug/win64/vc14/lib"
    )
endif()

# if(WIN32)
#     if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#         set(OPENCASCADE_DLL_DIR "${CMAKE_SOURCE_DIR}/external/opencascade/occt_vc14-64-with-debug/win64/vc14/bind")
#     else()
#         set(OPENCASCADE_DLL_DIR "${CMAKE_SOURCE_DIR}/external/opencascade/occt_vc14-64-with-debug/win64/vc14/bin")
#     endif()
    
#     # Add to PATH for debugging in Visual Studio
#     set_target_properties(opencascade_wrapper PROPERTIES
#         VS_DEBUGGER_ENVIRONMENT "PATH=${OPENCASCADE_DLL_DIR};$ENV{PATH}"
#     )
# endif()

target_link_libraries(opencascade_wrapper INTERFACE
    TKMath 
    TKernel 
    TKGeomBase 
    TKTopAlgo
)