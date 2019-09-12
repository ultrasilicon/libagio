cmake_minimum_required(VERSION 3.9)

get_filename_component(PROJECT_DIR_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
string(TOLOWER ${PROJECT_DIR_NAME} PROJECT_NAME)
project(${PROJECT_NAME}
	VERSION 0.1.0 # <major>.<minor>.<patch>.<tweak>
	LANGUAGES CXX
	DESCRIPTION "A cross-platform async C++ library with delicate OOP callback system.")


set(INCLUDE
	${PROJECT_SOURCE_DIR}/include/abstract_socket.h
	${PROJECT_SOURCE_DIR}/include/address.h
	${PROJECT_SOURCE_DIR}/include/async_event.h
	${PROJECT_SOURCE_DIR}/include/buffer.h
	${PROJECT_SOURCE_DIR}/include/check.h
	${PROJECT_SOURCE_DIR}/include/file.h
	${PROJECT_SOURCE_DIR}/include/function.h
	${PROJECT_SOURCE_DIR}/include/loop.h
	${PROJECT_SOURCE_DIR}/include/service.h
	${PROJECT_SOURCE_DIR}/include/stream.h
	${PROJECT_SOURCE_DIR}/include/tcp_server.h
	${PROJECT_SOURCE_DIR}/include/tcp_socket.h
	${PROJECT_SOURCE_DIR}/include/timer.h
	${PROJECT_SOURCE_DIR}/include/udp_socket.h
	${PROJECT_SOURCE_DIR}/include/utils.h
	${PROJECT_SOURCE_DIR}/include/agio.h
)

set(SRC
	${PROJECT_SOURCE_DIR}/src/abstract_socket.cpp
	${PROJECT_SOURCE_DIR}/src/address.cpp
	${PROJECT_SOURCE_DIR}/src/async_event.cpp
	${PROJECT_SOURCE_DIR}/src/buffer.cpp
	${PROJECT_SOURCE_DIR}/src/check.cpp
	${PROJECT_SOURCE_DIR}/src/file.cpp
	${PROJECT_SOURCE_DIR}/src/function.cpp
	${PROJECT_SOURCE_DIR}/src/loop.cpp
	${PROJECT_SOURCE_DIR}/src/service.cpp
	${PROJECT_SOURCE_DIR}/src/stream.cpp
	${PROJECT_SOURCE_DIR}/src/tcp_server.cpp
	${PROJECT_SOURCE_DIR}/src/tcp_socket.cpp
	${PROJECT_SOURCE_DIR}/src/timer.cpp
	${PROJECT_SOURCE_DIR}/src/udp_socket.cpp
	${PROJECT_SOURCE_DIR}/src/utils.cpp
	${PROJECT_SOURCE_DIR}/src/agio.cpp
)

include_directories(/usr/local/include)
include_directories(./include)



# compiler flags
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG -flto")

# If no build type is set, the default is Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)



if(UNIX)
find_package(PkgConfig)
pkg_check_modules(LIBUV REQUIRED libuv)
message("--   Include directory: " ${LIBUV_INCLUDE_DIRS})
message("--   Library directory: " ${LIBUV_LIBRARY_DIRS})
endif(UNIX)

add_library(${PROJECT_NAME} SHARED
	${INCLUDE} ${SRC}
)

target_link_libraries(${PROJECT_NAME} ${LIBUV_LIBRARIES})

# set_target_properties(${PROJECT_DIR_NAME} PROPERTIES SOVERSION 1)
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

# INSTALL
include(GNUInstallDirs)

configure_file(${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc
	DESTINATION lib/pkgconfig)

install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION lib)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
	DESTINATION include/${PROJECT_NAME}
	USE_SOURCE_PERMISSIONS
	FILES_MATCHING PATTERN "${PROJECT_DIR_NAME}/include/*.h")




