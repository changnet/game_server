TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += DEBUG

SOURCES += main.cpp \
    CNetBackend.cpp \
    CListenSocket.cpp \
    CTcpSocket.cpp

HEADERS += \
    CNetBackend.h \
    CListenSocket.h \
    gssocket.h \
    CTcpSocket.h

INCLUDEPATH += ../common \
               ../libraries/boost_1_56_0

LIBS += ../common/CLogger.o \
        ../common/CSingletonLog.o \
        ../common/CErrorLog.o \
        ../common/CPacket.o \
        ../common/CNetMessage.o \
        ../common/CServerSetting.o \
        ../common/CMessageQueue.o \
        ../common/CProcess.o \
        -lev
