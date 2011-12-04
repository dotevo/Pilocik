#include "pointselectionwindow.h"
#include "ui_pointselectionwindow.h"
#include "navigationwindow.h"
#include "routewindow.h"
#include "infowindow.h"
#include "twidgetmanager.h"
#include "routingmanager.h"

#include <QStandardItemModel>
#include <QAbstractItemView>
#include <QInputDialog>
#include <QVector>
#include <QMapIterator>
#include <QDateTime>
#include <QStringListModel>

#include <QDebug>
#include "osmscout/Searching.h"
#include "osmscout/Relation.h"
#include "osmscout/Routing.h"

#include <pilibocik/poifileppoi.h>
#include <pilibocik/boundarybox.h>
#include <pilibocik/position.h>

PointSelectionWindow::PointSelectionWindow(NavigationWindow *parent, double currentLocationX, double currentLocationY) :
    ui(new Ui::PointSelectionWindow),
    QFullScreenFrame(parent)
{
    searching = new osmscout::Searching();

    ui->setupUi(this);
    sizeChanged(NavigationWindow::main);

    PiLibocik::PoiFilePPOI poiDB;
    poiTypes = poiDB.loadPOIsTypesFromFile(Settings::getInstance()->getPoiFilePath());

    ui->typeComboBox->addItems(poiTypes.values());
    ui->typeComboBox->setCurrentIndex(0);

    ui->treeWidget->setColumnCount(COLUMNS_COUNT);
    ui->treeWidget->hideColumn(ID_COLUMN);
    ui->treeWidget->header()->setResizeMode(NAME_COLUMN, QHeaderView::ResizeToContents);
    ui->treeWidget->header()->setResizeMode(PATH_COLUMN, QHeaderView::Stretch);
    ui->treeWidget->header()->setResizeMode(INFO_COLUMN, QHeaderView::ResizeToContents);
    ui->treeWidget->header()->setResizeMode(FAV_COLUMN, QHeaderView::ResizeToContents);
    ui->treeWidget->header()->setStretchLastSection(false);

    QStringList headers;
    headers << "Id" << "Name" << "Info";
    ui->treeWidget->setHeaderLabels(headers);
    ui->treeWidget->header()->hide();

    ui->poiTreeWidget->setColumnCount(COLUMNS_COUNT);
    ui->poiTreeWidget->hideColumn(ID_COLUMN);
    ui->poiTreeWidget->header()->hide();
    ui->poiTreeWidget->header()->setResizeMode(NAME_COLUMN, QHeaderView::Stretch);
    ui->poiTreeWidget->header()->setResizeMode(PATH_COLUMN, QHeaderView::ResizeToContents);
    ui->poiTreeWidget->header()->setResizeMode(INFO_COLUMN, QHeaderView::ResizeToContents);
    ui->poiTreeWidget->header()->setResizeMode(FAV_COLUMN, QHeaderView::ResizeToContents);
    ui->poiTreeWidget->header()->setStretchLastSection(false);

    ui->typeLabel->setGeometry(0, 0, 10, 10);
    ui->typeComboBox->setGeometry(10, 10, 100, 100);

    ui->cityEditBtn->setVisible(false);
    ui->streetFrame->setVisible(false);
    ui->beforeFrame->setVisible(false);
    ui->nearestInfo->setVisible(false);

    if (currentLocationX != 0 && currentLocationY != 0) {
        currentLocation = QPointF(currentLocationX, currentLocationY);
    }

    initHistoryList();
    initFavouriteList();

    connect(ui->nameLineEdit, SIGNAL(opened()),
            this, SLOT(hide()));
    connect(ui->nameLineEdit, SIGNAL(closed()),
            this, SLOT(show()));

//    osmscout::LookupPOI lookup;

//    lookup.search("/home/bartek/osmscout-map/5poland/", 51.01, 16.95, 51.2, 17.05, "shop");
    fillPOIWidget(poiTypes.key(ui->typeComboBox->currentText()), ui->nameLineEdit->text());
}

PointSelectionWindow::~PointSelectionWindow(){
    delete ui;
}

void PointSelectionWindow::showNearestInfo(){
    ui->nearestInfo->setVisible(true);
}

void PointSelectionWindow::on_backBtn1_clicked(){
    emit back_clicked();
}

void PointSelectionWindow::on_backBtn2_clicked(){
    emit back_clicked();
}

void PointSelectionWindow::on_backBtn3_clicked(){
    emit back_clicked();
}

void PointSelectionWindow::on_backBtn4_clicked(){
    emit back_clicked();
}

void PointSelectionWindow::on_nearestButton_clicked(){
    npsw=new PointSelectionWindow(NavigationWindow::main,
                                 NavigationWindow::main->mapRenderer->getCoordinates().x(),
                                 NavigationWindow::main->mapRenderer->getCoordinates().y());
    npsw->setVisible(true);
    npsw->showNearestInfo();
    connect(npsw, SIGNAL(ok_clicked()), this, SLOT(pswClosed()));
    connect(npsw, SIGNAL(back_clicked()), this, SLOT(pswClosed()));
    connect(npsw, SIGNAL(positionChoosen(double,double,QString)), this, SLOT(nearestSet(double,double,QString)));
    setVisible(false);
}

void PointSelectionWindow::on_nearestClrButton_clicked(){
    ui->nearestButton->setText("Current position");
    currentLocation = NavigationWindow::main->getCoordinates();
    ui->poiTreeWidget->clear();
    fillPOIWidget(poiTypes.key(ui->typeComboBox->currentText()), ui->nameLineEdit->text());
}

void PointSelectionWindow::pswClosed(){
    npsw->close();
    delete npsw;
    setVisible(true);
}

void PointSelectionWindow::nearestSet(double lon, double lat, QString name)
{
    ui->nearestButton->setText(name);
    nearestPoint.first = name;
    nearestPoint.second.setLon(lon);
    nearestPoint.second.setLat(lat);
    currentLocation = QPointF(lon,lat);
    //Settings::getInstance()->addHistoryPoint(name, lon, lat);
    qDebug()<<lon<<lat<<name;
    ui->poiTreeWidget->clear();
    fillPOIWidget(poiTypes.key(ui->typeComboBox->currentText()), ui->nameLineEdit->text());
}

osmscout::AdminRegion PointSelectionWindow::searchRegion(const unsigned int id)
{
    osmscout::AdminRegion aRegion;  // empty region
    int index = -1;

    for (int i = 0; i < regions.size(); i++) {
        if (id == regions.at(i).reference.GetId()) {
            index = i;
            break;
        }
    }

    return (index >= 0 && index <regions.size()) ? regions.at(index) : aRegion;
}

osmscout::Location PointSelectionWindow::searchLocation(const QString locationName)
{
    int index = -1;

    for (int i = 0; i < locations.size(); i++) {
        if (locations.at(i).name.compare(locationName, Qt::CaseInsensitive) == 0) {
            index = i;
        }
    }

    osmscout::Location loc; // empty location

    return index >= 0 && index < locations.size() ? locations.at(index) : loc;
}

void PointSelectionWindow::on_okButton_clicked(){
//    emit ok_clicked();
}

void PointSelectionWindow::on_cityLineEdit_textChanged(const QString &text)
{

    osmscout::NodeRef n;

    searchingType = REGION_SEARCH;

    ui->treeWidget->clear();

    if (text.length() > 2)
    {
        regions = searching->searchRegion(text);

        int preceding = -1;

        for (int i = 0; i < regions.size(); i++)
        {
            QTreeWidgetItem *item;

            if (i == 0) {
                if (regions.at(i).name.startsWith(text, Qt::CaseInsensitive)) {
                    preceding = i;
                }
                item = new QTreeWidgetItem(ui->treeWidget);
            } else {
                if (regions.at(i).name.startsWith(text, Qt::CaseInsensitive)) {
                    item = new QTreeWidgetItem(ui->treeWidget, ui->treeWidget->topLevelItem(preceding));
                    preceding++;
                } else {
                    item = new QTreeWidgetItem(ui->treeWidget);
                }
            }

            item->setText(ID_COLUMN, QString::number(regions.at(i).reference.GetId()));
            item->setText(NAME_COLUMN, regions.at(i).name);
            QString path = "-";
            if (regions.at(i).path.size() > 0) {
                path = QString::fromUtf8(regions.at(i).path.front().c_str());
            }
            item->setText(PATH_COLUMN, path);
            item->setText(INFO_COLUMN, "INFO");
            item->setText(FAV_COLUMN, trUtf8("★"));

            ui->treeWidget->resizeColumnToContents(NAME_COLUMN);
            ui->treeWidget->resizeColumnToContents(PATH_COLUMN);

            //ui->treeWidget->setCurrentItem(item);
        }

        ui->treeWidget->setCurrentItem(ui->treeWidget->topLevelItem(0));
    }
}

void PointSelectionWindow::on_streetLineEdit_textChanged(const QString &text)
{
    searchingType = STREET_SEARCH;

    ui->treeWidget->clear();

    if (text.length() > 2)
    {

        locations = searching->searchLocation(text, region);

        int preceding = 0;

        for (int i = 0; i < locations.size(); i++)
        {
            QTreeWidgetItem *item;

            if (i == 0) {
                if (locations.at(i).name.startsWith(text, Qt::CaseInsensitive)) {
                    preceding = i;
                }
                item = new QTreeWidgetItem(ui->treeWidget);
            } else {
                if (locations.at(i).name.startsWith(text, Qt::CaseInsensitive)) {
                    item = new QTreeWidgetItem(ui->treeWidget, ui->treeWidget->topLevelItem(preceding));
                    preceding++;
                } else {
                    item = new QTreeWidgetItem(ui->treeWidget);
                }
            }

            // w sumie lipa, bo to jest id TYLKO pierwszej referencji do tej ulicy
            osmscout::ObjectRef loc = locations.at(i).references.front();
            item->setText(ID_COLUMN, QString::number(loc.GetId()));
            item->setText(NAME_COLUMN, locations.at(i).name);
            QString path = "-";
            if (locations.at(i).path.size() > 0) {
                path = locations.at(i).path.front();
            }
            item->setText(PATH_COLUMN, path);
            item->setText(INFO_COLUMN, "INFO");
            item->setText(FAV_COLUMN, trUtf8("★"));

            ui->treeWidget->resizeColumnToContents(NAME_COLUMN);
            ui->treeWidget->resizeColumnToContents(PATH_COLUMN);

            //ui->treeWidget->setCurrentItem(ui->treeWidget->topLevelItem(i));
        }

        ui->treeWidget->setCurrentItem(ui->treeWidget->topLevelItem(0));
    }
}

void PointSelectionWindow::on_nameLineEdit_textChanged(const QString &arg1)
{
    findPoisCount = 10;
    findPoisAreaLimit = 1;
    ui->poiTreeWidget->clear();
    fillPOIWidget(poiTypes.key(ui->typeComboBox->currentText()), arg1);
}

/*
  INFO click
  */
void PointSelectionWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    switch (searchingType) {
    case REGION_SEARCH:
        region = searchRegion(item->text(ID_COLUMN).toUInt());
        break;
    default: break;
    }

    if (column == INFO_COLUMN) {
        InfoWindow *infoWin = new InfoWindow(NavigationWindow::main);

        double lat;
        double lon;

        osmscout::NodeRef cityRef;

        switch (searchingType) {
        case REGION_SEARCH:
            infoWin = new InfoWindow(NavigationWindow::main);
            infoWin->resize(NavigationWindow::main->size());

            infoWin->setName(region.name);

            searching->searchNode(region.reference.GetId(), cityRef);

            lat = cityRef.Get()->GetLat();
            lon = cityRef.Get()->GetLon();

            retLon = lon;
            retLat = lat;
            retName = region.name;

            infoWin->setCoordinates(lon,lat);
            infoWin->setZoom(osmscout::magCity);
            infoWin->setVisible(true);
            //infoWin->setZoom(osmscout::magWorld);

            //osmscout::NodeRef node;
            //searching->database->GetNode(region.reference.GetId(), node);
            //infoWin->setCoordinates();

        break;

        case STREET_SEARCH:
            infoWin = new InfoWindow(NavigationWindow::main);
            infoWin->resize(NavigationWindow::main->size());
            osmscout::WayRef wayRef;

            int id = item->text(0).toInt();
            searching->searchWay(id, wayRef);

            infoWin->setName(QString::fromUtf8(wayRef.Get()->GetName().c_str()));

            wayRef.Get()->GetCenter(lat, lon);

            infoWin->setCoordinates(lon, lat);
            infoWin->setZoom(osmscout::magStreet);

            infoWin->setVisible(true);

            //infoWin->setMapRenderAreaSize(infoWin->getSize());
            //infoWin->setMapRender();
            //infoWin->forceMapWidgetRepaint();
            break;
        }

    }
    else if(column == FAV_COLUMN){

        double lat;
        double lon;

        osmscout::NodeRef cityRef;

        switch (searchingType) {
        case REGION_SEARCH:
            searching->searchNode(region.reference.GetId(), cityRef);

            lat = cityRef.Get()->GetLat();
            lon = cityRef.Get()->GetLon();

            Settings::getInstance()->addFavouritePoint(region.name, lon, lat);
            initFavouriteList();
        break;
        case STREET_SEARCH:

            osmscout::WayRef wayRef;

            int id = item->text(0).toInt();
            searching->searchWay(id, wayRef);

            wayRef.Get()->GetCenter(lat, lon);

            Settings::getInstance()->addFavouritePoint(cityName+", "+QString::fromUtf8(wayRef.Get()->GetName().c_str()), lon, lat);
            initFavouriteList();
            break;
        }

    } else {
        osmscout::NodeRef cityRef;

        switch (searchingType) {
        case REGION_SEARCH:
            searching->searchNode(region.reference.GetId(), cityRef);
            region = searchRegion(item->text(ID_COLUMN).toUInt());

            ui->cityLineEdit->setEnabled(false);
            ui->cityLineEdit->setText(region.name);
            ui->streetFrame->setVisible(true);

            retLon = cityRef.Get()->GetLon();
            retLat = cityRef.Get()->GetLat();
            retName = region.name;
            cityName = retName;

            ui->cityEditBtn->setVisible(true);
            ui->treeWidget->clear();
            break;

        case STREET_SEARCH:
            osmscout::WayRef wayRef;

            int id = item->text(0).toInt();
            searching->searchWay(id, wayRef);

            wayRef.Get()->GetCenter(retLat, retLon);
            retName = cityName + ", " + QString::fromUtf8(wayRef.Get()->GetName().c_str());


//            QString name = item->text(NAME_COLUMN);
//            location = searchLocation(name);

//            for (std::list<osmscout::ObjectRef>::const_iterator iter = location.references.begin();
//                 iter != location.references.end();
//                 ++iter) {

//                osmscout::ObjectRef objRef = *(iter);
//                osmscout::WayRef wayRef;

//                searching->searchWay(objRef.GetId(), wayRef);

//                if (wayRef.Valid()) {
//                    /*
//                    qDebug() << wayRef.Get()->EndIsJoint();
//                    qDebug() << QString::fromStdString(wayRef.Get()->GetAttributes().GetName());
//                    qDebug() << QString::fromStdString(wayRef.Get()->GetName());
//                    qDebug() << wayRef.Get()->GetId();
//                    qDebug() << wayRef.Get()->GetReferenceCount();
//                    qDebug() << QString::fromStdString(wayRef.Get()->GetRefName());
//                    qDebug() << wayRef.Get()->GetTagCount();
//                    qDebug() << wayRef.Get()->GetType();
//                    qDebug() << wayRef.Get()->GetWidth();
//                    */

//                    double lonMin;
//                    double lonMax;
//                    double latMin;
//                    double latMax;

//                    wayRef.Get()->GetBoundingBox(lonMin, lonMax, latMin, latMax);

//                    std::vector<osmscout::NodeRef> nodes;
//                    std::vector<osmscout::WayRef> ways;
//                    std::vector<osmscout::WayRef> areas;
//                    std::vector<osmscout::RelationRef> relationWays;
//                    std::vector<osmscout::RelationRef> relationAreas;

//                    //searching->searchObjects(lonMin, latMin, lonMax, latMax, nodes, ways, areas, relationWays, relationAreas);
///*
//                    qDebug() << "Nodes: " << nodes.size();
//                    qDebug() << "Ways: " << ways.size();
//                    qDebug() << "Areas: " << areas.size();
//                    qDebug() << "Relation ways: " << relationWays.size();
//                    qDebug() << "Relation areas: " << relationAreas.size();
//*/

//                    for (int i = 0; i < areas.size(); i++) {
//                        osmscout::WayRef areaRef;
//                        osmscout::WayRef area = areas.at(i);
//                        searching->searchWay(area.Get()->GetId(), areaRef);

//                        if (areaRef.Valid()) {
//                            // in poinst should be buildings!
//                            std::vector<osmscout::Point> points = areaRef.Get()->nodes;

//                            for (int j = 0; j < points.size(); j++) {
//                                osmscout::Point point = points.at(j);
//                                osmscout::NodeRef pointRef;
//                                searching->searchNode(point.GetId(), pointRef);

//                                if (pointRef.Valid()) {


//                                    //if (pointRef.Get()->GetTagCount() > 0)
//                                    //    qDebug() << pointRef.Get()->GetId() << " | " << QString::fromStdString(pointRef.Get()->GetTagValue(0));
//                               }

//                                osmscout::WayRef waj;
//                                searching->searchWay(point.GetId(), waj);

//                                if (waj.Valid()) {
//                                    for (int j = 0; j < waj.Get()->GetTagCount(); j++)
//                                        qDebug() << waj.Get()->GetTagKey(j) << " / " << QString(waj.Get()->GetTagValue(j).c_str());
//                                    if (waj.Get()->GetTagCount() > 0)
//                                        qDebug() << "\n";
//                                }

//                            }

//                        }
//                    }


//                    for (int i = 0; i < ways.size(); i++) {
//                        osmscout::WayRef way = ways.at(i);
//                        osmscout::WayRef way2;

//                        searching->searchWay(way.Get()->GetId(), way2);
//                        //qDebug() << QString::fromStdString(way2.Get()->GetTagValue(0));

//                        if (way2.Valid()) {
//                            std::vector<osmscout::Point> points = way.Get()->nodes;

//                            for (int j = 0; j < points.size(); j++) {
//                                osmscout::Point point = points.at(j);
//                                osmscout::NodeRef pointRef;
//                                searching->searchNode(point.GetId(), pointRef);

//                                osmscout::WayRef way3;
//                                searching->searchWay(point.GetId(), way3);

//                                if (way3.Valid()) {
//                                //   qDebug() << QString::fromStdString(way3.Get()->GetTagValue(0));
//                                }

//                                if (pointRef.Valid()) {

//                                    if (pointRef.Get()->GetTagCount() > 0)
//                                        qDebug() << pointRef.Get()->GetId() << " | " << QString(pointRef.Get()->GetTagValue(0).c_str());
//                               }

//                            }
//                        }


//                    }


//                }
//            }
            break;

        deafult: std::cerr << "Error! Searching type is not initialized!"; break;
        }
    }
}

void PointSelectionWindow::on_cityEditBtn_clicked(){
    if(!ui->cityLineEdit->isEnabled())
    {
        cityName = "";
        retLon = 0;
        retLat = 0;
        ui->cityLineEdit->setEnabled(true);
        ui->streetFrame->setVisible(false);
        ui->cityEditBtn->setVisible(false);
        ui->streetLineEdit->setText("");
        ui->cityLineEdit->setText(ui->cityLineEdit->text());
        on_cityLineEdit_textChanged(ui->cityLineEdit->text());
    }
}

void PointSelectionWindow::fillPOIWidget(int type, QString name)
{
    double cx = currentLocation.x();
    double cy = currentLocation.y();
    qDebug()<<cx<<cy;
    if(cx==0 && cy==0)
        return;

    PiLibocik::PoiFilePPOI poiDB;

    double x1 = cx;
    double x2 = cx;
    double y1 = cy;
    double y2 = cy;

    int limit = findPoisAreaLimit;
    int count = findPoisCount;
    if(ui->showOpened->isChecked()){
        limit *=10;
        count /= 2;
    }

    do{
        PiLibocik::BoundaryBox bbox(PiLibocik::Position(x1,y1),
                                    PiLibocik::Position(x2,y2));
        poiList = poiDB.loadPOIsFromFile(Settings::getInstance()->getPoiFilePath(), bbox, type);
        x1-=bbox.getCurrentError().first*10;
        y1-=bbox.getCurrentError().second*10;
        x2+=bbox.getCurrentError().first*10;
        y2+=bbox.getCurrentError().second*10;

        if(!name.isEmpty())
            for(int i=0; i<poiList.size(); i++)
            {
                if(!poiList.at(i).getName().contains(name, Qt::CaseInsensitive))
                {
                    poiList.removeAt(i);
                    i--;
                }
            }

        if(ui->showOpened->isChecked())
            for(int i=0; i<poiList.length(); i++)
            {
                PiLibocik::Poi poi = poiList.at(i);
                if(poi.isOpen(QDateTime::currentDateTime()) != 1)
                {
                    poiList.removeAt(i);
                    i--;
                }
            }

        limit--;
    }while(poiList.size()<count && limit>0);

    QMap<double, int> poisDistance;
    int i = 0;
    foreach(PiLibocik::Poi poi, poiList){
        poisDistance.insert(searching->CalculateDistance(cx, cy, poi.getLon(), poi.getLat()), i++);
    }
    QTreeWidgetItem *item;
    QMapIterator<double, int> it(poisDistance);
    while(it.hasNext())
    {
        it.next();
        item = new QTreeWidgetItem(ui->poiTreeWidget);
        item->setText(ID_COLUMN, QString::number(it.value()));
        QString distance = it.key() < 1 ? QString::number((int)(it.key()*1000)).append(" m") : QString::number(it.key(), 'f', 2).append(" km");
        item->setText(NAME_COLUMN, poiList.at(it.value()).getName());
        item->setText(PATH_COLUMN, distance);
        item->setText(INFO_COLUMN, "INFO");
        item->setText(FAV_COLUMN, trUtf8("★"));
    }
}

void PointSelectionWindow::on_findMore_clicked()
{
    int currSize = ui->poiTreeWidget->topLevelItemCount();
    while(ui->poiTreeWidget->topLevelItemCount()<=currSize && findPoisAreaLimit < 10)
    {
        if(currSize >= findPoisCount)
            findPoisCount+=10;
        else
            findPoisAreaLimit++;
        ui->poiTreeWidget->clear();
        fillPOIWidget(poiTypes.key(ui->typeComboBox->currentText()), ui->nameLineEdit->text());
    }
}

void PointSelectionWindow::on_showOpened_stateChanged(int state)
{
    ui->poiTreeWidget->clear();
    fillPOIWidget(poiTypes.key(ui->typeComboBox->currentText()), ui->nameLineEdit->text());
}

void PointSelectionWindow::on_poiOK_clicked() {
<<<<<<< HEAD

    NavigationWindow *par = qobject_cast<NavigationWindow*>(parent());

    //par->setRoute(QVector<osmscout::Routing::Step>::fromStdVector(route));

    PiLibocik::Position p1(17.0300428, 51.0814354);
    PiLibocik::Position p2(17.0392651, 51.0761212);
    //osmscout::Routing r(RoutingManager::getInstance()->getPartitionFile());
    QList<osmscout::Routing::Step> route = NavigationWindow::main->routeWin->routingManager->getRouting()->CalculateRoute(p1, p2);
    //QList<osmscout::Routing::Step> route = par->routeWin->routingManager->getRouting()->CalculateRoute(p1, p2);

    par->setRoute(route);

    TWidgetManager::getInstance()->setRouting(true);

    //par->setRoute(QList<osmscout::Routing::Step>::fromStdVector(route));
=======
    //int selectedId = ui->poiTreeWidget->currentItem()->text(ID_COLUMN).toInt();
>>>>>>> fa05be7f8086d72672445a254294efcb2fd4f556

    if(retLon!=0 && retLat!=0 && !retName.isEmpty())
        emit positionChoosen(retLon, retLat, retName);
    emit ok_clicked();
}

void PointSelectionWindow::on_addressOK_clicked()
{
    if(retLon!=0 && retLat!=0 && !retName.isEmpty())
        emit positionChoosen(retLon, retLat, retName);
    emit ok_clicked();
}

void PointSelectionWindow::on_historyOK_clicked()
{
    if(retLon!=0 && retLat!=0 && !retName.isEmpty())
        emit positionChoosen(retLon, retLat, retName);
    emit ok_clicked();
}

void PointSelectionWindow::on_favouriteOK_clicked()
{
    if(retLon!=0 && retLat!=0 && !retName.isEmpty())
        emit positionChoosen(retLon, retLat, retName);
    emit ok_clicked();
}

void PointSelectionWindow::on_typeComboBox_currentIndexChanged(const QString &type)
{
    findPoisCount = 10;
    findPoisAreaLimit = 1;
    ui->poiTreeWidget->clear();
    fillPOIWidget(poiTypes.key(type), ui->nameLineEdit->text());
}

void PointSelectionWindow::on_poiTreeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    PiLibocik::Poi poi = poiList.at(item->text(ID_COLUMN).toInt());
    if (column == INFO_COLUMN) {
        InfoWindow *infoPoiWin = new InfoWindow(NavigationWindow::main);
        infoPoiWin->resize(NavigationWindow::main->size());

        double lat,lon;
        QString name;

        lat = poi.getLat();
        lon = poi.getLon();
        name = poi.getName();
        infoPoiWin->setDetails(poi.getTags());
        infoPoiWin->setZoom(50000);

//            osmscout::NodeRef node;
//            searching->searchNode(item->text(ID_COLUMN).toInt(), node);

//            lat = node.Get()->GetLat();
//            lon = node.Get()->GetLon();
//            name = item->text(NAME_COLUMN);
//            infoPoiWin->setZoom(osmscout::magBuilding);

        infoPoiWin->setName(name);
        infoPoiWin->setCoordinates(lon, lat);

        infoPoiWin->setVisible(true);
    } else if (column == FAV_COLUMN) {
        Settings::getInstance()->addFavouritePoint("POI: "+poi.getName(), poi.getLon(), poi.getLat());
        initFavouriteList();
    }
}

void PointSelectionWindow::on_poiTreeWidget_itemSelectionChanged()
{
    if(ui->poiTreeWidget->selectedItems().size()==0)
        return;
    PiLibocik::Poi poi = poiList.at(ui->poiTreeWidget->selectedItems().at(0)->text(0).toInt());
    retLon = poi.getLon();
    retLat = poi.getLat();
    retName = "POI: "+poi.getName();
}

void PointSelectionWindow::on_treeWidget_itemSelectionChanged()
{

}

void PointSelectionWindow::on_historyList_itemSelectionChanged()
{
    if(ui->historyList->selectedItems().size()==0)
        return;
    StorePoint sp = historyPoints.at(ui->historyList->selectedItems().at(0)->text(0).toInt());
    retLon = sp.getLon();
    retLat = sp.getLat();
    retName = sp.getName();
}

void PointSelectionWindow::on_favouriteList_itemSelectionChanged()
{
    if(ui->favouriteList->selectedItems().size()==0)
        return;
    StorePoint sp = favouritePoints.at(ui->favouriteList->selectedItems().at(0)->text(0).toInt());
    retLon = sp.getLon();
    retLat = sp.getLat();
    retName = sp.getName();
}

void PointSelectionWindow::on_historyList_itemClicked(QTreeWidgetItem *item, int column)
{
    if (column == INFO_COLUMN) {
        StorePoint sp = historyPoints.at(item->text(0).toInt());

        InfoWindow *infoPoiWin = new InfoWindow(NavigationWindow::main);
        infoPoiWin->resize(NavigationWindow::main->size());

        infoPoiWin->setZoom(50000);

        infoPoiWin->setName(sp.getName());
        infoPoiWin->setCoordinates(sp.getLon(), sp.getLat());

        infoPoiWin->setVisible(true);
    } else if (column == FAV_COLUMN) {
        StorePoint sp = historyPoints.at(item->text(0).toInt());
        Settings::getInstance()->addFavouritePoint(sp.getName(), sp.getLon(), sp.getLat());
        initFavouriteList();
    }
}

void PointSelectionWindow::on_favouriteList_itemClicked(QTreeWidgetItem *item, int column)
{
    if (column == INFO_COLUMN) {
        StorePoint sp = favouritePoints.at(item->text(0).toInt());

        InfoWindow *infoPoiWin = new InfoWindow(NavigationWindow::main);
        infoPoiWin->resize(NavigationWindow::main->size());

        infoPoiWin->setZoom(50000);

        infoPoiWin->setName(sp.getName());
        infoPoiWin->setCoordinates(sp.getLon(), sp.getLat());

        infoPoiWin->setVisible(true);
    } else if (column == FAV_COLUMN) {
        Settings::getInstance()->removeFavouritePoint(item->text(0).toInt());
        initFavouriteList();
    }
}

void PointSelectionWindow::on_poiTreeWidget_clicked(const QModelIndex &index)
{

}

void PointSelectionWindow::on_tabWidget_currentChanged(int index)
{
    ui->poiTab->setFocus();
    retLon = 0;
    retLat = 0;
    retName.clear();
}

void PointSelectionWindow::initHistoryList()
{
    historyPoints = Settings::getInstance()->getHistoryPoints();
    ui->historyList->clear();
    ui->historyList->setColumnCount(COLUMNS_COUNT);
    ui->historyList->hideColumn(ID_COLUMN);
    ui->historyList->header()->hide();
    foreach(StorePoint sp, historyPoints)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->historyList);
        item->setText(ID_COLUMN, QString::number(sp.getPos()));
        item->setText(NAME_COLUMN, sp.getName());
        double numDist = searching->CalculateDistance(currentLocation.x(), currentLocation.y(), sp.getLon(), sp.getLat());
        QString distance = numDist < 1 ? QString::number((int)(numDist*1000)).append(" m") : QString::number(numDist, 'f', 2).append(" km");
        item->setText(PATH_COLUMN, distance);
        item->setText(INFO_COLUMN, "INFO");
        item->setText(FAV_COLUMN, trUtf8("★"));
    }
    ui->historyList->header()->setResizeMode(NAME_COLUMN, QHeaderView::Stretch);
    ui->historyList->header()->setResizeMode(PATH_COLUMN, QHeaderView::ResizeToContents);
    ui->historyList->header()->setResizeMode(INFO_COLUMN, QHeaderView::ResizeToContents);
    ui->historyList->header()->setResizeMode(FAV_COLUMN, QHeaderView::ResizeToContents);
    ui->historyList->header()->setStretchLastSection(false);
}

void PointSelectionWindow::initFavouriteList()
{
    favouritePoints = Settings::getInstance()->getFavouritePoints();
    ui->favouriteList->clear();
    ui->favouriteList->setColumnCount(COLUMNS_COUNT);
    ui->favouriteList->hideColumn(ID_COLUMN);
    ui->favouriteList->header()->hide();
    foreach(StorePoint sp, favouritePoints)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->favouriteList);
        item->setText(ID_COLUMN, QString::number(sp.getPos()));
        item->setText(NAME_COLUMN, sp.getName());
        double numDist = searching->CalculateDistance(currentLocation.x(), currentLocation.y(), sp.getLon(), sp.getLat());
        QString distance = numDist < 1 ? QString::number((int)(numDist*1000)).append(" m") : QString::number(numDist, 'f', 2).append(" km");
        item->setText(PATH_COLUMN, distance);
        item->setText(INFO_COLUMN, "INFO");
        item->setText(FAV_COLUMN, trUtf8("☒"));
    }
    ui->favouriteList->header()->setStretchLastSection(false);
    ui->favouriteList->header()->setResizeMode(NAME_COLUMN, QHeaderView::Stretch);
    ui->favouriteList->header()->setResizeMode(PATH_COLUMN, QHeaderView::ResizeToContents);
    ui->favouriteList->header()->setResizeMode(INFO_COLUMN, QHeaderView::ResizeToContents);
    ui->historyList->header()->setResizeMode(FAV_COLUMN, QHeaderView::ResizeToContents);
}

void PointSelectionWindow::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
