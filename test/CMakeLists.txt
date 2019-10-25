add_executable(
    tests
    test_protocol_utils.cpp
)

target_link_libraries(
    tests
    gtest_main
    agio
)

add_test(
  NAME unit
  COMMAND ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/unit_tests
)
