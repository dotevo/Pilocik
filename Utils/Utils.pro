 #-------------------------------------------------
#
# Project created by QtCreator 2011-10-25T14:16:57
#
#-------------------------------------------------

QT       += core gui \
            sql \
            xml

TARGET = Utils
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    databasegen.cpp \
    gpsemulator.cpp \
    partitionsrenderwidget.cpp

HEADERS  += mainwindow.h \
    databasegen.h \
    gpsemulator.h \
    partitionsrenderwidget.h

FORMS    += mainwindow.ui

TRANSLATIONS = lang\pl.ts









