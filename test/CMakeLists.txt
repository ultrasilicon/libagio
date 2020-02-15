link_directories(${CMAKE_BINARY_DIR}/Debug)
include_directories(${PROJECT_SOURCE_DIR}/include)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC -g -DDEBUG")

add_executable(
    unit_tests
    test_protocol_utils.cpp
    test_buffer.cpp
    test_file.cpp
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
