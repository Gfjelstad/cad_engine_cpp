# ==============================================================================
# Third-party Dependencies
# ==============================================================================
function(link_opencascade_dlls target_name)
    set(OPENCASCADE_ROOT "${CMAKE_SOURCE_DIR}/external/opencascade")

    # Core DLLs
    file(GLOB OCCT_DLLS
        "${OPENCASCADE_ROOT}/occt_vc14-64-with-debug/win64/vc14/bind/*.dll"
    )
    # 3rd party DLLs
    file(GLOB_RECURSE THIRD_PARTY_DLLS_TBB
        "${OPENCASCADE_ROOT}/3rdparty-vc14-64/*/bin/*.dll"
    )
    # file(GLOB_RECURSE THIRD_PARTY_DLLS_JEMALLOC
    #     "${OPENCASCADE_ROOT}/3rdparty-vc14-64/jemalloc*/bin/*.dll"
    # )

    set(ALL_DLLS ${OCCT_DLLS} ${THIRD_PARTY_DLLS_TBB})

    list(REMOVE_DUPLICATES ALL_DLLS)

    foreach(DLL ${ALL_DLLS})
        if(NOT EXISTS "${DLL}")
            message(WARNING "DLL not found: ${DLL}")
        else()
            # message(STATUS "DLL found: ${DLL}")
            add_custom_command(TARGET ${target_name} POST_BUILD
                # COMMAND ${CMAKE_COMMAND} -E echo "Copying DLL ${DLL} to $<TARGET_FILE_DIR:${target_name}>"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${DLL}" "$<TARGET_FILE_DIR:${target_name}>"
            )
        endif()
    endforeach()
endfunction()
# function(link_opencascade_dlls target_name)
#     set(OPENCASCADE_ROOT "${CMAKE_SOURCE_DIR}/external/opencascade")

#     # OCCT core DLLs
#     file(GLOB OCCT_DLLS
#         "${OPENCASCADE_ROOT}/occt_vc14-64-with-debug/win64/bind/*.dll"
#     )
 

#     # 3rd-party DLLs
#     file(GLOB_RECURSE THIRD_PARTY_DLLS_TBB
#         "${OPENCASCADE_ROOT}/3rdparty-vc14-64/tbb*/bin/*.dll"
#     )
#     file(GLOB_RECURSE THIRD_PARTY_DLLS_JEMALLOC
#         "${OPENCASCADE_ROOT}/3rdparty-vc14-64/jemalloc*/bin/*.dll"
#     )

#     set(ALL_DLLS ${OCCT_DLLS} ${THIRD_PARTY_DLLS_TBB} ${THIRD_PARTY_DLLS_JEMALLOC})

#     foreach(DLL ${ALL_DLLS})
#     add_custom_command(TARGET ${target_name} POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E echo "Copying DLL ${DLL} to $<TARGET_FILE_DIR:${target_name}>"
#         COMMAND ${CMAKE_COMMAND} -E copy_if_different "${DLL}" "$<TARGET_FILE_DIR:${target_name}>"
#     )
# endforeach()
# endfunction()

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


target_link_libraries(opencascade_wrapper INTERFACE
    TKMath 
    TKernel 
    TKGeomBase 
    TKTopAlgo
    TKBRep
    TKGeomAlgo       # Required for geometric algorithms
    TKG3d            # Required for 3D geometry
    TKBO             # Required for boolean operations
    TKPrim           # Required for primitive operations
    TKDESTEP        
    TKDESTL        
    TKMesh    
)