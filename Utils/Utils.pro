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


Debug:unix:LIBS += ../lib/PiLibocik/debug/libPiLibocik.so
Debug:win32:LIBS += ../lib/PiLibocik/debug/libPiLibocik.a

Release:unix:LIBS += ../lib/PiLibocik/release/libPiLibocik.so
Release:win32:LIBS += ../lib/PiLibocik/release/libPiLibocik.a

SOURCES += main.cpp\
        mainwindow.cpp \
    databasegen.cpp \
    gpsemulator.cpp \
    partitionsrenderwidget.cpp \
    ../lib/sosmscout/src/osmscout/WayIndex.cpp \
    ../lib/sosmscout/src/osmscout/Way.cpp \
    ../lib/sosmscout/src/osmscout/WaterIndex.cpp \
    ../lib/sosmscout/src/osmscout/Util.cpp \
    ../lib/sosmscout/src/osmscout/Types.cpp \
    ../lib/sosmscout/src/osmscout/TypeConfigLoader.cpp \
    ../lib/sosmscout/src/osmscout/TypeConfig.cpp \
    ../lib/sosmscout/src/osmscout/Tag.cpp \
    ../lib/sosmscout/src/osmscout/StyleConfigLoader.cpp \
    ../lib/sosmscout/src/osmscout/StyleConfig.cpp \
    ../lib/sosmscout/src/osmscout/SegmentAttributes.cpp \
    ../lib/sosmscout/src/osmscout/RoutingProfile.cpp \
    ../lib/sosmscout/src/osmscout/Routing.cpp \
    ../lib/sosmscout/src/osmscout/Route.cpp \
    ../lib/sosmscout/src/osmscout/Relation.cpp \
    ../lib/sosmscout/src/osmscout/Progress.cpp \
    ../lib/sosmscout/src/osmscout/Point.cpp \
    ../lib/sosmscout/src/osmscout/Partitionmodel.cpp \
    ../lib/sosmscout/src/osmscout/Partitioning.cpp \
    ../lib/sosmscout/src/osmscout/OptimizeLowZoom.cpp \
    ../lib/sosmscout/src/osmscout/ObjectRef.cpp \
    ../lib/sosmscout/src/osmscout/NumericIndex.cpp \
    ../lib/sosmscout/src/osmscout/NodeUseIndex.cpp \
    ../lib/sosmscout/src/osmscout/NodeIndex.cpp \
    ../lib/sosmscout/src/osmscout/Node.cpp \
    ../lib/sosmscout/src/osmscout/MapPainter.cpp \
    ../lib/sosmscout/src/osmscout/Location.cpp \
    ../lib/sosmscout/src/osmscout/GroundTile.cpp \
    ../lib/sosmscout/src/osmscout/Database.cpp \
    ../lib/sosmscout/src/osmscout/CityStreetIndex.cpp \
    ../lib/sosmscout/src/osmscout/AreaWayIndex.cpp \
    ../lib/sosmscout/src/osmscout/AreaNodeIndex.cpp \
    ../lib/sosmscout/src/osmscout/AreaAreaIndex.cpp \
    ../lib/sosmscout/src/osmscout/AdminRegion.cpp \
    ../lib/sosmscout/src/osmscout/oss/Scanner.cpp \
    ../lib/sosmscout/src/osmscout/oss/Parser.cpp \
    ../lib/sosmscout/src/osmscout/ost/ScannerT.cpp \
    ../lib/sosmscout/src/osmscout/ost/ParserT.cpp \
    ../lib/sosmscout/src/osmscout/util/Transformation.cpp \
    ../lib/sosmscout/src/osmscout/util/String.cpp \
    ../lib/sosmscout/src/osmscout/util/StopClock.cpp \
    ../lib/sosmscout/src/osmscout/util/Reference.cpp \
    ../lib/sosmscout/src/osmscout/util/Projection.cpp \
    ../lib/sosmscout/src/osmscout/util/Geometry.cpp \
    ../lib/sosmscout/src/osmscout/util/FileWriter.cpp \
    ../lib/sosmscout/src/osmscout/util/FileScanner.cpp \
    ../lib/sosmscout/src/osmscout/util/Cache.cpp


HEADERS  += mainwindow.h \
    databasegen.h \
    gpsemulator.h \
    partitionsrenderwidget.h

FORMS    += mainwindow.ui

INCLUDEPATH += ../lib/sosmscout/include \
        ../lib/PiLibocik/include

TRANSLATIONS = lang/pl.ts













