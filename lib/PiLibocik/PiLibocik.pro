#-------------------------------------------------
#
# Project created by QtCreator 2011-11-08T13:01:11
#
#-------------------------------------------------
QT      -= gui
QT       += sql xml

TARGET = PiLibocik
TEMPLATE = app
CONFIG += staticlib

DEFINES += PiLibocik_WRITE_MODE

SOURCES += src/pilibocik.cpp \
    src/geohash.cpp \
    src/poi.cpp \
    src/preparedata.cpp \
    src/boundarybox.cpp \
    src/poifileppoi.cpp \
    main.cpp \
    src/position.cpp \
    src/partition/edge.cpp \
    src/partition/node.cpp \
    src/partition/partitionfile.cpp \
    src/partition/restriction.cpp \
    src/partition/way.cpp \
    src/partition/boundaryedge.cpp \
    src/partition/routenode.cpp


INCLUDEPATH +=include

HEADERS += include/pilibocik/pilibocik.h \
    include/pilibocik/geohash.h \
    include/pilibocik/poi.h \
    include/pilibocik/preparedata.h \
    include/pilibocik/boundarybox.h \
    include/pilibocik/poifileppoi.h \
    include/pilibocik/position.h \
    include/pilibocik/partition/edge.h \
    include/pilibocik/partition/node.h \
    include/pilibocik/partition/partitionfile.h \
    include/pilibocik/partition/restriction.h \
    include/pilibocik/partition/routenode.h \
    include/pilibocik/partition/way.h \
    include/pilibocik/partition/boundaryedge.h


unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}




