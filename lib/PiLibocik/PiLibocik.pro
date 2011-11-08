#-------------------------------------------------
#
# Project created by QtCreator 2011-11-08T13:01:11
#
#-------------------------------------------------

QT       -= gui sql xml

TARGET = PiLibocik
TEMPLATE = lib
CONFIG += staticlib

DEFINES += PiLibocik_WRITE_MODE

SOURCES += src/pilibocik.cpp \
    src/geohash.cpp \
    src/point.cpp \
    src/poi.cpp \
    src/preparedata.cpp
    src/boundarybox.cpp

INCLUDEPATH +=include

HEADERS += include/pilibocik/pilibocik.h \
    include/pilibocik/geohash.h \
    include/pilibocik/point.h \
    include/pilibocik/poi.h \
    include/pilibocik/preparedata.h
    include/pilibocik/boundarybox.h


unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}






















