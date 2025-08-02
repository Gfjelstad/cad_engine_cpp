function(add_nodus_test name)
    add_executable(${name} ${ARGN})

    target_include_directories(${name} PRIVATE
        "${CMAKE_SOURCE_DIR}/external/doctest"
    )

    target_link_libraries(${name} PRIVATE
        nodus_rpc_handler
        nodus_core
        opencascade_wrapper
    )

    add_test(NAME ${name} COMMAND ${name})
endfunction()

add_nodus_test(json_rpc_tests
    json_rpc_tests.cpp
)
add_nodus_test(rpc_handler_tests
    rpc_handler_tests.cpp
)
add_nodus_test(nodus_integration_tests
    nodus_integration_tests.cpp
)
