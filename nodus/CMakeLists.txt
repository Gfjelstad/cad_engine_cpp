# Main Application
add_executable(nodus_app main.cpp)

target_link_libraries(nodus_app PRIVATE
    # nodus_cad_core
    nodus_rpc_handler  # References library from nodusComLib/
    opencascade_wrapper
)