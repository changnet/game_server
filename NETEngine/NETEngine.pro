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
        ../common/CNetMessage.o \
        ../common/CServerSetting.o \
        ../common/CMessageQueue.o \
        ../common/CUtility.o \
        ../common/CLogStream.o \
        ../common/CRuntimeStream.o \
        -lev
