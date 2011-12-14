#-------------------------------------------------
#
# Project created by QtCreator 2011-12-03T11:42:08
#
#-------------------------------------------------

QT       += core gui network xml

LIBS += D:\Projects\Pilocik\!!NEW\Pilocik\Setup\activesync\Lib\rapi.lib

TARGET = Setup
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    newsframe.cpp \
    filedownload.cpp \
    mapdownloadframe.cpp \
    activesynccomm.cpp \
    installframe.cpp \
    resourcesmanager.cpp \
    installer.cpp \
    stylesframe.cpp \
    updateframe.cpp \
    pluginsframe.cpp

HEADERS  += mainwindow.h \
    newsframe.h \
    filedownload.h \
    mapdownloadframe.h \
    activesynccomm.h \
    installframe.h \
    resourcesmanager.h \
    installer.h \
    stylesframe.h \
    updateframe.h \
    pluginsframe.h

FORMS    += mainwindow.ui \
    newsframe.ui \
    mapdownloadframe.ui \
    installframe.ui \
    stylesframe.ui \
    updateframe.ui \
    pluginsframe.ui

INCLUDEPATH += activesync\inc

RESOURCES += \
    res.qrc











































