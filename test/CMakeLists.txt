link_directories(${CMAKE_BINARY_DIR}/Debug)
include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(
    unit_tests
    test_protocol_utils.cpp
)

target_link_libraries(
    unit_tests
    gtest
    gtest_main
    agio-d
)

add_test(
  NAME unit_tests
  COMMAND ${CMAKE_BINARY_DIR}/bin/unit_tests
)
