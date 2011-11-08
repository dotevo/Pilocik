#-------------------------------------------------
#
# Project created by QtCreator 2011-11-08T13:01:11
#
#-------------------------------------------------

QT       -= gui

TARGET = PiLibocik
TEMPLATE = lib
CONFIG += staticlib

SOURCES += src/pilibocik.cpp

INCLUDEPATH +=include

HEADERS += include/pilibocik/pilibocik.h


unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
