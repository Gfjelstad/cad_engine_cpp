add_library(nodus_rpc_handler STATIC
    listener.cpp
)

target_include_directories(nodus_rpc_handler PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(nodus_rpc_handler PUBLIC 
    nlohmann_json::nlohmann_json
)
