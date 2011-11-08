#-------------------------------------------------
#
# Project created by QtCreator 2011-11-08T13:01:11
#
#-------------------------------------------------

QT       -= gui

TARGET = PiLibocik
TEMPLATE = lib
CONFIG += staticlib

SOURCES += src/pilibocik.cpp \
    src/geohash.cpp \
    src/point.cpp \
    src/poi.cpp

INCLUDEPATH +=include

HEADERS += include/pilibocik/pilibocik.h \
    include/pilibocik/geohash.h \
    include/pilibocik/point.h \
    include/pilibocik/poi.h


unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}










