TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += DEBUG

SOURCES += main.cpp

INCLUDEPATH += ../common \
               ../libraries/boost_1_56_0/

LIBS += ../common/CLogger.o \
        ../common/CSingletonLog.o \
        ../common/CServerSetting.o \
        ../common/CErrorLog.o \
        ../common/CProcess.o \
        -lev
