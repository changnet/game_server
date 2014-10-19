TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    CDataBackend.cpp

HEADERS += \
    CDataBackend.h

INCLUDEPATH += ../common \
               ../libraries/boost_1_56_0

LIBS += ../common/CLogger.o \
        ../common/CSingletonLog.o \
        ../common/CErrorLog.o \
        ../common/CServerSetting.o \
        ../common/CPacket.o \
        ../common/CNetMessage.o \
        ../common/CProcess.o \
        -pthread \            #not -lpthread
        -lev
