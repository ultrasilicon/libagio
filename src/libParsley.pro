#-------------------------------------------------
#
# Project created by QtCreator 2018-03-20T12:11:24
#
#-------------------------------------------------

QT       -= core gui

TARGET = Parsley
TEMPLATE = lib
CONFIG += dylib

SOURCES += \
	PAbstractSocket.cpp \
	PAddress.cpp \
	PTcpServer.cpp \
	PTcpSocket.cpp \
	PTimer.cpp \
	PUdpSocket.cpp \
	PUtils.cpp \
	Parsley.cpp \
    PFile.cpp \
    PCheck.cpp

HEADERS += \
	PAbstractSocket.h \
	PAddress.h \
	PTcpServer.h \
	PTcpSocket.h \
	PTimer.h \
	PUdpSocket.h \
	PUtils.h \
	Parsley.h \
    PFile.h \
    PCheck.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libuv
