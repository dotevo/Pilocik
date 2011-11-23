#-------------------------------------------------
#
# Project created by QtCreator 2011-11-08T17:36:36
#
#-------------------------------------------------

QT       += core gui sql xml

TARGET = test
TEMPLATE = app

DEFINES += PiLibocik_WRITE_MODE


SOURCES += main.cpp\
        mainwindow.cpp \
    ../src/preparedata.cpp \
    ../src/position.cpp \
    ../src/poi.cpp \
    ../src/pilibocik.cpp \
    ../src/poifileppoi.cpp \
    ../src/boundarybox.cpp \
    ../src/geohash.cpp

HEADERS  += mainwindow.h \

FORMS    += mainwindow.ui

INCLUDEPATH += ../include/

OTHER_FILES += \
    config.xml



