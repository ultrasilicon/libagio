TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../../../include

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
    ../../../src/PUtils.cpp \
    ../../../src/PFunction.cpp \
    ../../../src/PBuffer.cpp


HEADERS += \
    ../../../include/PAbstractSocket.h \
    ../../../include/PAddress.h \
    ../../../include/Parsley.h \
    ../../../include/PAsync.h \
    ../../../include/PCheck.h \
    ../../../include/PFile.h \
    ../../../include/PTcpServer.h \
    ../../../include/PTcpSocket.h \
    ../../../include/PTimer.h \
    ../../../include/PUdpSocket.h \
    ../../../include/PUtils.h \
    ../../../include/PFunction.h \
    ../../../include/PBuffer.h

mac {
    PKG_CONFIG = /usr/local/bin/pkg-config
}

CONFIG += link_pkgconfig
PKGCONFIG += libuv
PKGCONFIG += parsly
