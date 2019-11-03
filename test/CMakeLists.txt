add_executable(
    unit_tests
    test_protocol_utils.cpp
)

target_link_libraries(
    unit_tests
    gtest_main
    agio
)

add_test(
  NAME unit_tests
  COMMAND ${CMAKE_BINARY_DIR}/bin/unit_tests
)
