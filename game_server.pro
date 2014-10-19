TEMPLATE = subdirs

SUBDIRS += \
    common \
    LOGEngine \
    NETEngine \
    OSLancher \
    DataServer

CONFIG += ordered    #CONFIG选项要求各个子项目按顺序编译，子目录的编译顺序在SUBDIRS中指明

