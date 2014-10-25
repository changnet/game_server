TEMPLATE = app
CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    CLogBackend.cpp

INCLUDEPATH += ../common \
               ../libraries/boost_1_56_0

LIBS += ../common/CLogger.o \
        ../common/CLogMessage.o \
        ../common/CServerSetting.o \
        ../common/CPacket.o \
        ../common/CNetMessage.o \
        ../common/CProcess.o \
        ../common/CSeamphoreLock.o \
        ../common/CSharedMemory.o \
        ../common/CLogFile.o \
        -lev \
        -pthread \   #not -lpthread
        -lrt

HEADERS += \
    CLogBackend.h
