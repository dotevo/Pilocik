#-------------------------------------------------
#
# Project created by QtCreator 2011-09-27T08:39:56
#
#-------------------------------------------------

QT       += core gui

TARGET = ../../build/Pilocik
TEMPLATE = app


SOURCES += main.cpp\
    tmovableframe.cpp \
    twidgetmanager.cpp \
    widgets/tclockwidget.cpp \
    navigationwindow.cpp \
    routewindow.cpp \
    pointselectionwindow.cpp \
    infowindow.cpp \
    optionswindow.cpp \
    mapoptionswindow.cpp \
    useroptionswindow.cpp \
    gpsoptionswindow.cpp \
    keyboardwindow.cpp \
    qfullscreenframe.cpp

HEADERS  += \
    tmovableframe.h \
    twidgetmanager.h \
    widgets/tclockwidget.h \
    navigationwindow.h \
    routewindow.h \
    pointselectionwindow.h \
    infowindow.h \
    optionswindow.h \
    mapoptionswindow.h \
    useroptionswindow.h \
    gpsoptionswindow.h \
    keyboardwindow.h \
    qfullscreenframe.h

FORMS    += \
    widgets/tclockwidget.ui \
    navigationwindow.ui \
    routewindow.ui \
    pointselectionwindow.ui \
    infowindow.ui \
    optionswindow.ui \
    mapoptionswindow.ui \
    useroptionswindow.ui \
    gpsoptionswindow.ui \
    keyboardwindow.ui

INCLUDEPATH += ./


Release:DESTDIR = ../build/release
Release:OBJECTS_DIR = ../build/release/.obj
Release:MOC_DIR = ../build/release/.moc
Release:RCC_DIR = ../build/release/.rcc
Release:UI_DIR = ../build/release/.ui

Debug:DESTDIR = ../build/debug
Debug:OBJECTS_DIR = ../build/debug/.obj
Debug:MOC_DIR = ../build/debug/.moc
Debug:RCC_DIR = ../build/debug/.rcc
Debug:UI_DIR = ../build/debug/.ui

RESOURCES +=


























































