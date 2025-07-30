# enable_testing()
# cmake_minimum_required(VERSION 3.16)
# project(MyCADApp)

# set(CMAKE_CXX_STANDARD 17)

# # Point to OpenCASCADE config
# set(OpenCASCADE_DIR "${CMAKE_SOURCE_DIR}/opencascade/occ/cmake")
# find_package(OpenCASCADE REQUIRED)

# # Optional: manually include if needed
# include_directories("${OpenCASCADE_INCLUDE_DIR}")
# include_directories("${CMAKE_SOURCE_DIR}/opencascade/occ/inc")

# # Link directories
# if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#     link_directories("${CMAKE_SOURCE_DIR}/opencascade/occ/win64/vc14/libd")
# else()
#     link_directories("${CMAKE_SOURCE_DIR}/opencascade/occ/win64/vc14/lib")
# endif()

# # nlohmann/json
# include(FetchContent)
# FetchContent_Declare(
#   json
#   GIT_REPOSITORY https://github.com/nlohmann/json.git
#   GIT_TAG v3.11.3
# )
# FetchContent_MakeAvailable(json)

# add_library(my_cad_lib INTERFACE)

# # Include paths
# target_include_directories(my_cad_lib INTERFACE
#     ${OpenCASCADE_INCLUDE_DIR}
#     "${CMAKE_SOURCE_DIR}/opencascade/occ/inc"
#     "${CMAKE_SOURCE_DIR}/include"
# )

# # Link OpenCASCADE and json
# target_link_libraries(my_cad_lib INTERFACE
#     TKMath TKernel TKGeomBase TKTopAlgo
#     nlohmann_json::nlohmann_json
# )


# add_executable(my_app main.cpp)
# target_link_libraries(my_app PRIVATE my_cad_lib)

# add_executable(test_handler tests/test_handler.cpp)
# target_link_libraries(test_handler PRIVATE my_cad_lib)

# add_test(NAME test_handler COMMAND test_handler)

# # add_executable(test_handler tests/test_handler.cpp)
# # target_include_directories(test_handler PRIVATE include)
# # target_link_libraries(test_handler rpc_handler)
# # # Your executable
# # add_executable(my_app main.cpp)

# # # Link OCC libs
# # target_link_libraries(my_app PRIVATE TKMath TKernel TKGeomBase TKTopAlgo nlohmann_json::nlohmann_json)


# new


cmake_minimum_required(VERSION 3.16)
project(MyCADApp)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

# OpenCASCADE
set(OpenCASCADE_DIR "${CMAKE_SOURCE_DIR}/opencascade/occ/cmake")
find_package(OpenCASCADE REQUIRED)
include_directories("${OpenCASCADE_INCLUDE_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/opencascade/occ/inc")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    link_directories("${CMAKE_SOURCE_DIR}/opencascade/occ/win64/vc14/libd")
else()
    link_directories("${CMAKE_SOURCE_DIR}/opencascade/occ/win64/vc14/lib")
endif()

# nlohmann/json
include(FetchContent)
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

# Compile your rpc_handler library from source
add_library(rpc_handler STATIC
    rpc/handler.cpp
)

# Make sure includes and json are visible to rpc_handler and consumers
target_include_directories(rpc_handler PUBLIC
    "${CMAKE_SOURCE_DIR}/include"
)

target_link_libraries(rpc_handler PUBLIC nlohmann_json::nlohmann_json)

# Create an INTERFACE library for OpenCASCADE + your includes, for easy reuse
add_library(my_cad_lib INTERFACE)

target_include_directories(my_cad_lib INTERFACE
    ${OpenCASCADE_INCLUDE_DIR}
    "${CMAKE_SOURCE_DIR}/opencascade/occ/inc"
    "${CMAKE_SOURCE_DIR}/include"
)

target_link_libraries(my_cad_lib INTERFACE
    TKMath TKernel TKGeomBase TKTopAlgo
)

# Your main app executable links both my_cad_lib and rpc_handler
add_executable(my_app main.cpp)

target_link_libraries(my_app PRIVATE
    my_cad_lib
    rpc_handler
)

# Your test executable links both my_cad_lib and rpc_handler
add_executable(test_handler tests/test_handler.cpp)

target_include_directories(test_handler PRIVATE
    "${CMAKE_SOURCE_DIR}/include/external"
)

target_link_libraries(test_handler PRIVATE
    my_cad_lib
    rpc_handler
)

# Register test with CTest
add_test(NAME test_handler COMMAND test_handler)
