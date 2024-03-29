#-------------------------------------------------
#
# Project created by QtCreator 2011-09-27T08:39:56
#
#-------------------------------------------------

QT       += core gui sql \
            xml

TARGET = ../../build/Pilocik
TEMPLATE = app

#LIBS += ../lib/PiLibocik/debug/libPiLibocik.a
Debug:LIBS += ../lib/PiLibocik/debug/libPiLibocik.a
Release:LIBS += ../lib/PiLibocik/release/libPiLibocik.a

TRANSLATIONS = lang/pl_PL.tr \
        lang/en_EN.tr


SOURCES += main.cpp\
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
    qfullscreenframe.cpp \
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
    ../lib/sosmscout/src/osmscout/Partitioning.cpp \
    ../lib/sosmscout/src/osmscout/Partitionmodel.cpp \
    ../lib/sosmscout/src/osmscout/Searching.cpp \
    ../lib/sosmscout/src/osmscout/Point.cpp \
    ../lib/sosmscout/src/osmscout/Routing.cpp \
    mappainterqt.cpp \
    tlineedit.cpp \
    gpsinfowindow.cpp \
    pluginswindow.cpp \
    gpsreceiver.cpp \
    widgets/tspeedmeterwidget.cpp \
    settings.cpp \
    widgets/tsliderwidget.cpp \
    widgets/thintwidget.cpp \
    creditswindow.cpp \
    widgets/troutingprogresswidget.cpp \
    widgets/terrorwidget.cpp \
    widgets/thandymenuwidget.cpp \
    routingmanager.cpp \
    pluginmanager.cpp\
    gpsdata.cpp \
    widgets/tpoiinfowidget.cpp


HEADERS  +=  ../lib/sosmscout/include/osmscout/Partitioning.h \
    ../lib/sosmscout/include/osmscout/Routing.h \
    tmovableframe.h \
    twidgetmanager.h \
    widgets/tclockwidget.h \
    navigationwindow.h \
    maprenderwidget.h \
    routewindow.h \
    pointselectionwindow.h \
    infowindow.h \
    optionswindow.h \
    mapoptionswindow.h \
    useroptionswindow.h \
    gpsoptionswindow.h \
    keyboardwindow.h \
    qfullscreenframe.h \
    tlineedit.h \
    gpsinfowindow.h \
    gpsreceiver.h \
    widgets/tspeedmeterwidget.h \
    settings.h \
    widgets/tsliderwidget.h \
    widgets/thintwidget.h \
    creditswindow.h \
    widgets/troutingprogresswidget.h \
    widgets/thandymenuwidget.h \
    routingmanager.h \
    pluginmanager.h \
    widgets/terrorwidget.h \
    pluginwidget.h \
    plugininterface.h \
    pluginswindow.h \
    gpsdata.h \
    widgets/tpoiinfowidget.h


FORMS    += \
    widgets/tclockwidget.ui \
    navigationwindow.ui \
    routewindow.ui \
    infowindow.ui \
    optionswindow.ui \
    mapoptionswindow.ui \
    useroptionswindow.ui \
    gpsoptionswindow.ui \
    keyboardwindow.ui \
    gpsinfowindow.ui \
    widgets/tspeedmeterwidget.ui \
    pointselectionwindow.ui \
    widgets/tsliderwidget.ui \
    widgets/thintwidget.ui \
    creditswindow.ui \
    widgets/troutingprogresswidget.ui \
    widgets/terrorwidget.ui \
    widgets/thandymenuwidget.ui \
    pluginswindow.ui \
    widgets/tpoiinfowidget.ui

INCLUDEPATH += ./
INCLUDEPATH += ./../lib/sosmscout/include/
INCLUDEPATH += ./../lib/PiLibocik/include/

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

OTHER_FILES += \
    settings.xml



