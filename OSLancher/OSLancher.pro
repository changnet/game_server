TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += DEBUG

SOURCES += main.cpp

INCLUDEPATH += ../common \
               ../libraries/boost_1_56_0/

LIBS += ../common/CLogger.o \
        ../common/CServerSetting.o \
        ../common/CLogMessage.o \
        ../common/CProcess.o \
        ../common/CLogFile.o \
        ../common/CPacket.o \
        -lev
