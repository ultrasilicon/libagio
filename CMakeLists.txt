cmake_minimum_required(VERSION 3.9)

# Project
#############################################################
get_filename_component(PROJECT_DIR_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
project(${PROJECT_DIR_NAME}
	VERSION 0.1.0 # <major>.<minor>.<patch>.<tweak>
	LANGUAGES CXX
	DESCRIPTION "A cross-platform async C++ library with delicate OOP callback system.")

set(INCLUDE
	${PROJECT_SOURCE_DIR}/include/PAbstractSocket.h
	${PROJECT_SOURCE_DIR}/include/PAddress.h
	${PROJECT_SOURCE_DIR}/include/PAsync.h
	${PROJECT_SOURCE_DIR}/include/PBuffer.h
	${PROJECT_SOURCE_DIR}/include/PCheck.h
	${PROJECT_SOURCE_DIR}/include/PFile.h
	${PROJECT_SOURCE_DIR}/include/PFunction.h
	${PROJECT_SOURCE_DIR}/include/PTcpServer.h
	${PROJECT_SOURCE_DIR}/include/PTcpSocket.h
	${PROJECT_SOURCE_DIR}/include/PTimer.h
	${PROJECT_SOURCE_DIR}/include/PUdpSocket.h
	${PROJECT_SOURCE_DIR}/include/PUtils.h
	${PROJECT_SOURCE_DIR}/include/Parsley.h
)

set(SRC
	${PROJECT_SOURCE_DIR}/src/PAbstractSocket.cpp
	${PROJECT_SOURCE_DIR}/src/PAddress.cpp
	${PROJECT_SOURCE_DIR}/src/PAsync.cpp
	${PROJECT_SOURCE_DIR}/src/PBuffer.cpp
	${PROJECT_SOURCE_DIR}/src/PCheck.cpp
	${PROJECT_SOURCE_DIR}/src/PFile.cpp
	${PROJECT_SOURCE_DIR}/src/PFunction.cpp
	${PROJECT_SOURCE_DIR}/src/PTcpServer.cpp
	${PROJECT_SOURCE_DIR}/src/PTcpSocket.cpp
	${PROJECT_SOURCE_DIR}/src/PTimer.cpp
	${PROJECT_SOURCE_DIR}/src/PUdpSocket.cpp
	${PROJECT_SOURCE_DIR}/src/PUtils.cpp
	${PROJECT_SOURCE_DIR}/src/Parsley.cpp
)

include_directories(/usr/local/include)
include_directories(./include)


# Build Flags
#############################################################

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

# Dependencies
#############################################################
if(UNIX)
find_package(PkgConfig)
pkg_check_modules(LIBUV REQUIRED libuv)
endif(UNIX)



#if(NOT LIBUV)
#    message(FATAL_ERROR "libuv was not found.")
#endif(NOT LIBUV)


add_library(${PROJECT_DIR_NAME} SHARED
	${INCLUDE} ${SRC}
)

target_link_libraries(${PROJECT_DIR_NAME} uv)

# set_target_properties(${PROJECT_DIR_NAME} PROPERTIES SOVERSION 1)
set_target_properties(${PROJECT_DIR_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

# INSTALL
include(GNUInstallDirs)
install(TARGETS ${PROJECT_DIR_NAME}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
	DESTINATION include/${PROJECT_DIR_NAME}
	FILES_MATCHING PATTERN "${PROJECT_DIR_NAME}/include/*.h"
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)

if(NOT CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX /usr/local)
endif(NOT CMAKE_INSTALL_PREFIX)

configure_file(${PROJECT_DIR_NAME}.pc.in ${PROJECT_DIR_NAME}.pc @ONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_DIR_NAME}.pc
	DESTINATION lib/pkgconfig
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)






















