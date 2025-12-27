add_library(nodus_core STATIC
    geometry/LineNode.cpp
    geometry/Node.cpp
    geometry/SketchNode.cpp
    geometry/ExtrudeNode.cpp
    Model.cpp
)

target_include_directories(nodus_core PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(nodus_core PUBLIC 
    opencascade_wrapper
    nlohmann_json::nlohmann_json
)

link_opencascade_dlls(nodus_core)