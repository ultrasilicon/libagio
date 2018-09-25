TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    ../../../src/PAbstractSocket.cpp \
    ../../../src/Parsley.cpp \
    ../../../src/PAsync.cpp \
    ../../../src/PCheck.cpp \
    ../../../src/PFile.cpp \
    ../../../src/PTcpServer.cpp \
    ../../../src/PTcpSocket.cpp \
    ../../../src/PTimer.cpp \
    ../../../src/PUdpSocket.cpp \
    ../../../src/PAddress.cpp \
    ../../../src/PUtils.cpp

HEADERS += \
    ../../../src/PAbstractSocket.h \
    ../../../src/PAddress.h \
    ../../../src/Parsley.h \
    ../../../src/PAsync.h \
    ../../../src/PCheck.h \
    ../../../src/PFile.h \
    ../../../src/PTcpServer.h \
    ../../../src/PTcpSocket.h \
    ../../../src/PTimer.h \
    ../../../src/PUdpSocket.h \
    ../../../src/PUtils.h

mac {
    PKG_CONFIG = /usr/local/bin/pkg-config
}

CONFIG += link_pkgconfig
PKGCONFIG += libuv
