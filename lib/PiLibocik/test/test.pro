#-------------------------------------------------
#
# Project created by QtCreator 2011-11-08T17:36:36
#
#-------------------------------------------------

QT       += core gui sql xml

TARGET = test
TEMPLATE = app

DEFINES += PiLibocik_WRITE_MODE

Debug:unix:LIBS += ../debug/libPiLibocik.so
Debug:win32:LIBS += ../debug/libPiLibocik.a

Release:unix:LIBS += ../release/libPiLibocik.so
Release:win32:LIBS += ../release/libPiLibocik.a


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \

FORMS    += mainwindow.ui

INCLUDEPATH += ../include/

OTHER_FILES += \
    config.xml



