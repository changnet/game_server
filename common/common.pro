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
    CProcess.cpp \
    CSeamphoreLock.cpp \
    CSharedMemory.cpp \
    CRWSharedMemory.cpp \
    CRorWSharedMemory.cpp \
    CMessageQueue.cpp \
    CPacket.cpp \
    CNetMessage.cpp \
    CLogMessage.cpp \
    CLogFile.cpp

HEADERS += \
    CLogger.h \
    gstypes.h \
    gslog.h \
    CServerSetting.h \
    CProcess.h \
    CSeamphoreLock.h \
    CSharedMemory.h \
    CRWSharedMemory.h \
    CRorWSharedMemory.h \
    CMessageQueue.h \
    CPacket.h \
    CNetMessage.h \
    CLogMessage.h \
    CLogFile.h

