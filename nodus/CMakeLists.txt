# Main Application
add_executable(nodus_app main.cpp)

target_link_libraries(nodus_app PRIVATE
    nodus_rpc_handler
    opencascade_wrapper
    nodus_core
)

# setup_opencascade_dlls(nodus_app)
link_opencascade_dlls(nodus_app)