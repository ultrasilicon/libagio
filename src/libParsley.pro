#-------------------------------------------------
#
# Project created by QtCreator 2018-03-20T12:11:24
#
#-------------------------------------------------

QT       -= core gui

TARGET = Parsley
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
	PAbstractSocket.cpp \
	PAddress.cpp \
	PLoop.cpp \
	PTcpServer.cpp \
	PTcpSocket.cpp \
	PTimer.cpp \
	PUdpSocket.cpp \
	PUtils.cpp \
	Parsley.cpp

HEADERS += \
	PAbstractSocket.h \
	PAddress.h \
	PLoop.h \
	PTcpServer.h \
	PTcpSocket.h \
	PTimer.h \
	PUdpSocket.h \
	PUtils.h \
	Parsley.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libuv
