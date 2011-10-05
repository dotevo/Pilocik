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
    navigationwindow.cpp \
    maprenderwidget.cpp \
    ../lib/sosmscout/src/osmscout/Database.cpp \
    ../lib/sosmscout/src/osmscout/StyleConfigLoader.cpp \
    ../lib/sosmscout/src/osmscout/Util.cpp \
    ../lib/sosmscout/src/osmscout/util/Geometry.cpp \
    ../lib/sosmscout/src/osmscout/StyleConfig.cpp \
    ../lib/sosmscout/src/osmscout/TypeConfig.cpp \
    ../lib/sosmscout/src/osmscout/OptimizeLowZoom.cpp \
    ../lib/sosmscout/src/osmscout/NodeIndex.cpp \
    ../lib/sosmscout/src/osmscout/WayIndex.cpp \
    ../lib/sosmscout/src/osmscout/AreaAreaIndex.cpp \
    ../lib/sosmscout/src/osmscout/AreaNodeIndex.cpp \
    ../lib/sosmscout/src/osmscout/AreaWayIndex.cpp \
    ../lib/sosmscout/src/osmscout/CityStreetIndex.cpp \
    ../lib/sosmscout/src/osmscout/NodeUseIndex.cpp \
    ../lib/sosmscout/src/osmscout/WaterIndex.cpp \
    ../lib/sosmscout/src/osmscout/Route.cpp \
    ../lib/sosmscout/src/osmscout/util/Cache.cpp \
    ../lib/sosmscout/src/osmscout/RoutingProfile.cpp \
    ../lib/sosmscout/src/osmscout/TypeConfigLoader.cpp \
    ../lib/sosmscout/src/osmscout/util/StopClock.cpp \
    ../lib/sosmscout/src/osmscout/oss/Parser.cpp \
    ../lib/sosmscout/src/osmscout/oss/Scanner.cpp \
    ../lib/sosmscout/src/osmscout/Point.cpp \
    ../lib/sosmscout/src/osmscout/util/Projection.cpp \
    ../lib/sosmscout/src/osmscout/Types.cpp \
    ../lib/sosmscout/src/osmscout/Tag.cpp \
    ../lib/sosmscout/src/osmscout/util/Reference.cpp \
    ../lib/sosmscout/src/osmscout/Node.cpp \
    ../lib/sosmscout/src/osmscout/Relation.cpp \
    ../lib/sosmscout/src/osmscout/Way.cpp \
    ../lib/sosmscout/src/osmscout/util/FileScanner.cpp \
    ../lib/sosmscout/src/osmscout/util/String.cpp \
    ../lib/sosmscout/src/osmscout/util/Transformation.cpp \
    ../lib/sosmscout/src/osmscout/NumericIndex.cpp \
    ../lib/sosmscout/src/osmscout/AdminRegion.cpp \
    ../lib/sosmscout/src/osmscout/Location.cpp \
    ../lib/sosmscout/src/osmscout/GroundTile.cpp \
    ../lib/sosmscout/src/osmscout/util/FileWriter.cpp \
    ../lib/sosmscout/src/osmscout/SegmentAttributes.cpp \
    ../lib/sosmscout/src/osmscout/ObjectRef.cpp \
    ../lib/sosmscout/src/osmscout/Progress.cpp \
    ../lib/sosmscout/src/osmscout/ost/ParserT.cpp \
    ../lib/sosmscout/src/osmscout/ost/ScannerT.cpp \
    ../lib/sosmscout/src/osmscout/MapPainter.cpp \
    mappainterqt.cpp


HEADERS  += menuwindow.h \
    tmovableframe.h \
    twidget.h \
    twidgetmanager.h \
    widgets/tclockwidget.h \
    navigationwindow.h \
    maprenderwidget.h

FORMS    += menuwindow.ui \
    widgets/tclockwidget.ui \
    navigationwindow.ui

INCLUDEPATH += ./
INCLUDEPATH += ./../lib/sosmscout/include/


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

RESOURCES += \
    icons.qrc

































