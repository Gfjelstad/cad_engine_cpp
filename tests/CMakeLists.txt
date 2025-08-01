add_executable(nodus_rpc_tests 
    test_handler.cpp
)



target_include_directories(nodus_rpc_tests PRIVATE
    "${CMAKE_SOURCE_DIR}/external/doctest"
)

target_link_libraries(nodus_rpc_tests PRIVATE
nodus_rpc_handler
# nodus_cad_core
)

# Register tests with CTest
add_test(NAME rpc_handler_tests COMMAND nodus_rpc_tests)
