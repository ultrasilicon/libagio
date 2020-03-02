cmake_minimum_required(VERSION 3.9)


##### Project Config #############################################################
get_filename_component(PROJECT_DIR_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
string(TOLOWER ${PROJECT_DIR_NAME} PROJECT_NAME)
project(${PROJECT_NAME}
	VERSION 0.1.0 # <major>.<minor>.<patch>.<tweak>
	LANGUAGES CXX
	DESCRIPTION "A cross-platform async I/O library for C++ agile development")


##### Source Files ###############################################################
set(INCLUDE
	${PROJECT_SOURCE_DIR}/include/abstract_socket.h
	${PROJECT_SOURCE_DIR}/include/address.h
	${PROJECT_SOURCE_DIR}/include/async_event.h
	${PROJECT_SOURCE_DIR}/include/buffer.h
	${PROJECT_SOURCE_DIR}/include/callback.h
	${PROJECT_SOURCE_DIR}/include/callback_async.h
	${PROJECT_SOURCE_DIR}/include/check.h
	${PROJECT_SOURCE_DIR}/include/file.h
	${PROJECT_SOURCE_DIR}/include/lib.h
	${PROJECT_SOURCE_DIR}/include/loop.h
	${PROJECT_SOURCE_DIR}/include/promise.h 
	${PROJECT_SOURCE_DIR}/include/proto_engine.h
	${PROJECT_SOURCE_DIR}/include/proto_utils.h
	${PROJECT_SOURCE_DIR}/include/service.h
	${PROJECT_SOURCE_DIR}/include/stream.h
	${PROJECT_SOURCE_DIR}/include/tcp_server.h
	${PROJECT_SOURCE_DIR}/include/tcp_socket.h
	${PROJECT_SOURCE_DIR}/include/timer.h
	${PROJECT_SOURCE_DIR}/include/udp_socket.h
	${PROJECT_SOURCE_DIR}/include/utils.h
	${PROJECT_SOURCE_DIR}/include/variant.h
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
    ${PROJECT_SOURCE_DIR}/src/lib.cpp
    ${PROJECT_SOURCE_DIR}/src/loop.cpp
	${PROJECT_SOURCE_DIR}/src/promise.cpp
	${PROJECT_SOURCE_DIR}/src/proto_engine.cpp
	${PROJECT_SOURCE_DIR}/src/service.cpp
	${PROJECT_SOURCE_DIR}/src/stream.cpp
	${PROJECT_SOURCE_DIR}/src/tcp_server.cpp
	${PROJECT_SOURCE_DIR}/src/tcp_socket.cpp
	${PROJECT_SOURCE_DIR}/src/timer.cpp
	${PROJECT_SOURCE_DIR}/src/udp_socket.cpp
	${PROJECT_SOURCE_DIR}/src/utils.cpp
	${PROJECT_SOURCE_DIR}/src/agio.cpp
)


##### Paths ######################################################################
if(APPLE) # macOS mojave 10.14+ needs link dir and include dir
    link_directories(/usr/local/lib)
    include_directories(/usr/local/include)
endif()
include_directories(./include)


##### Compiler Flags #############################################################
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG -flto")


##### Build Options ##############################################################
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_DEBUG_POSTFIX -d)
endif()
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
message("-- Output directory: " ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})


##### Dependencies ###############################################################
if(UNIX)
find_package(PkgConfig)
pkg_check_modules(LIBUV REQUIRED libuv)
message("--   Include directory: " ${LIBUV_INCLUDE_DIRS})
message("--   Library directory: " ${LIBUV_LIBRARY_DIRS})
endif(UNIX)


##### Target #####################################################################
add_library(${PROJECT_NAME} SHARED ${INCLUDE} ${SRC} )
target_link_libraries(${PROJECT_NAME} ${LIBUV_LIBRARIES})
# set_target_properties(${PROJECT_DIR_NAME} PROPERTIES SOVERSION 1)
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})


##### Install ####################################################################
configure_file(${PROJECT_NAME}.pc.in ${PROJECT_NAME}${CMAKE_DEBUG_POSTFIX}.pc @ONLY)
if(UNIX)
	install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}${CMAKE_DEBUG_POSTFIX}.pc
		DESTINATION lib/pkgconfig)
endif()
if(LINUX)
	install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}${CMAKE_DEBUG_POSTFIX}.pc
        DESTINATION /usr/lib/pkgconfig)
endif()

install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION lib)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
	DESTINATION include/${PROJECT_NAME}
	USE_SOURCE_PERMISSIONS
	FILES_MATCHING PATTERN "${PROJECT_DIR_NAME}/include/*.h")


##### Tests ######################################################################
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	enable_testing()
	add_subdirectory(test)
	add_test(NAME test COMMAND test)
endif()
