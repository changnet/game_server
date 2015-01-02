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
        ../common/CUtility.o \
        ../common/CLogStream.o \
        ../common/CRuntimeStream.o \
        -lev

HEADERS += \
    main.h
