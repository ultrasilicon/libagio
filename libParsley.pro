#-------------------------------------------------
#
# Project created by QtCreator 2018-03-20T12:11:24
#
#-------------------------------------------------

QT       -= core gui

TARGET = Parsley
TEMPLATE = lib
CONFIG += c++1z


INCLUDEPATH += ./include

SOURCES += \
    ./src/AbstractSocket.cpp \
    ./src/Address.cpp \
    ./src/TcpServer.cpp \
    ./src/TcpSocket.cpp \
    ./src/Timer.cpp \
    ./src/UdpSocket.cpp \
    ./src/Utils.cpp \
    ./src/arsley.cpp \
    ./src/File.cpp \
    ./src/Check.cpp \
    ./src/Async.cpp \
    ./src/Function.cpp

HEADERS += \
    ./include/AbstractSocket.h \
    ./include/Address.h \
    ./include/TcpServer.h \
    ./include/TcpSocket.h \
    ./include/Timer.h \
    ./include/UdpSocket.h \
    ./include/Utils.h \
    ./include/arsley.h \
    ./include/File.h \
    ./include/Check.h \
    ./include/Async.h \
    ./include/Function.h

mac {
    PKG_CONFIG = /usr/local/bin/pkg-config
    CONFIG += dylib
}

unix {
    target.path = /usr/lib
    INSTALLS += target
    CONFIG += link_pkgconfig
    PKGCONFIG += libuv
}

