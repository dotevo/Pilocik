#-------------------------------------------------
#
# Project created by QtCreator 2011-09-27T08:39:56
#
#-------------------------------------------------

QT       += core gui

TARGET = ../../build/Pilocik
TEMPLATE = app


SOURCES += main.cpp\
        menuwindow.cpp \
    tmovableframe.cpp \
    twidget.cpp \
    twidgetmanager.cpp \
    widgets/tclockwidget.cpp \
    navigationwindow.cpp

HEADERS  += menuwindow.h \
    tmovableframe.h \
    twidget.h \
    twidgetmanager.h \
    widgets/tclockwidget.h \
    navigationwindow.h

FORMS    += menuwindow.ui \
    widgets/tclockwidget.ui \
    navigationwindow.ui

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



























