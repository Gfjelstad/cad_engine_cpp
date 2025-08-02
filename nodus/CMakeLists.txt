# Main Application
add_executable(nodus_app main.cpp)

target_link_libraries(nodus_app PRIVATE
    nodus_rpc_handler
    opencascade_wrapper
    nodus_core
)