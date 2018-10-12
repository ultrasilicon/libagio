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
    ./src/PAbstractSocket.cpp \
    ./src/PAddress.cpp \
    ./src/PTcpServer.cpp \
    ./src/PTcpSocket.cpp \
    ./src/PTimer.cpp \
    ./src/PUdpSocket.cpp \
    ./src/PUtils.cpp \
    ./src/Parsley.cpp \
    ./src/PFile.cpp \
    ./src/PCheck.cpp \
    ./src/PAsync.cpp \
    PFunction.cpp

HEADERS += \
    ./include/PAbstractSocket.h \
    ./include/PAddress.h \
    ./include/PTcpServer.h \
    ./include/PTcpSocket.h \
    ./include/PTimer.h \
    ./include/PUdpSocket.h \
    ./include/PUtils.h \
    ./include/Parsley.h \
    ./include/PFile.h \
    ./include/PCheck.h \
    ./include/PAsync.h \
    PFunction.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += libuv
}

mac {
    CONFIG += dylib
}
