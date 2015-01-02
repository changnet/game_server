TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += DEBUG

INCLUDEPATH += ../libraries/boost_1_56_0
LIBS += -lev \
        -pthread \            #not -lpthread
        -lrt

SOURCES += main.cpp \
    CLogger.cpp \
    CServerSetting.cpp \
    CSeamphoreLock.cpp \
    CSharedMemory.cpp \
    CRWSharedMemory.cpp \
    CRorWSharedMemory.cpp \
    CMessageQueue.cpp \
    CNetMessage.cpp \
    CLogMessage.cpp \
    CUtility.cpp \
    CLogWorker.cpp \
    CBackend.cpp \
    CLogStream.cpp \
    CRuntimeStream.cpp

HEADERS += \
    CLogger.h \
    gstypes.h \
    gslog.h \
    CServerSetting.h \
    CSeamphoreLock.h \
    CSharedMemory.h \
    CRWSharedMemory.h \
    CRorWSharedMemory.h \
    CMessageQueue.h \
    CPacket.h \
    CNetMessage.h \
    CLogMessage.h \
    CUtility.h \
    CLogWorker.h \
    CBackend.h \
    gssignal.h \
    CLogStream.h \
    CRuntimeStream.h

