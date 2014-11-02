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
    CTcpSocket.h \
    main.h

INCLUDEPATH += ../common \
               ../libraries/boost_1_56_0

LIBS += ../common/CLogger.o \
        ../common/CLogMessage.o \
        ../common/CPacket.o \
        ../common/CNetMessage.o \
        ../common/CServerSetting.o \
        ../common/CMessageQueue.o \
        ../common/CLogFile.o \
        ../common/CUtility.o \
        -lev
