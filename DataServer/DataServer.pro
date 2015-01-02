TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    CDataBackend.cpp

HEADERS += \
    CDataBackend.h \
    main.h

INCLUDEPATH += ../common \
               ../libraries/boost_1_56_0

LIBS += ../common/CLogger.o \
        ../common/CLogMessage.o \
        ../common/CServerSetting.o \
        ../common/CNetMessage.o \
        ../common/CUtility.o \
        ../common/CLogWorker.o \
        ../common/CSeamphoreLock.o \
        ../common/CRorWSharedMemory.o \
        ../common/CSharedMemory.o \
        ../common/CBackend.o \
        ../common/CLogStream.o \
        ../common/CRuntimeStream.o \
        -lrt \
        -pthread \            #not -lpthread
        -lev
