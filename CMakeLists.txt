cmake_minimum_required(VERSION 2.8.8)

# Project
#############################################################
get_filename_component(PROJECT_DIR_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
string(REPLACE " " "_" PROJECT_DIR_NAME ${PROJECT_DIR_NAME})

project(${PROJECT_DIR_NAME}
        VERSION 1.0.0.0 # <major>.<minor>.<patch>.<tweak>
        LANGUAGES CXX)

string(SUBSTRING ${PROJECT_NAME} 3 -1 LIB_NAME)
string(TOLOWER ${LIB_NAME} LIB_NAME)


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

# prepare pkg-config file
if(UNIX)
set(PKGCONFIG
    ${PROJECT_SOURCE_DIR}/pkg-config/libparsley.pc
)
endif(UNIX)

# Build Flags
#############################################################

# compiler flags
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 1z)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# debug
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")

# release
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






















