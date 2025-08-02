cmake_minimum_required(VERSION 3.16)
project(nodus VERSION 1.0.0 LANGUAGES CXX)

# ==============================================================================
# Project Configuration
# ==============================================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()


add_subdirectory(external)

add_subdirectory(nodusComLib)
add_subdirectory(nodusCoreLib)
add_subdirectory(nodus)

enable_testing()
add_subdirectory(tests)