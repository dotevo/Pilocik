#include "pointselectionwindow.h"
#include "ui_pointselectionwindow.h"
#include "navigationwindow.h"
#include "infowindow.h"

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

    QStringList headers;
    headers << "Id" << "Name" << "Info";
    ui->treeWidget->setHeaderLabels(headers);
    ui->treeWidget->header()->hide();

    ui->poiTreeWidget->setColumnCount(COLUMNS_COUNT);
    ui->poiTreeWidget->hideColumn(ID_COLUMN);
    ui->poiTreeWidget->header()->hide();

    ui->typeLabel->setGeometry(0, 0, 10, 10);
    ui->typeComboBox->setGeometry(10, 10, 100, 100);

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

}

PointSelectionWindow::~PointSelectionWindow(){
    delete ui;
}

osmscout::AdminRegion PointSelectionWindow::searchRegion(const int id)
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

void PointSelectionWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if (column == NAME_COLUMN || column == PATH_COLUMN) {
        // TODO:
        //ui->cityLineEdit->setText(item->text(column));

        switch (searchingType) {
        case REGION_SEARCH:
            region = searchRegion(item->text(ID_COLUMN).toInt());
            break;

        case STREET_SEARCH:
            QString name = item->text(NAME_COLUMN);
            location = searchLocation(name);

            for (std::list<osmscout::ObjectRef>::const_iterator iter = location.references.begin();
                 iter != location.references.end();
                 ++iter) {

                osmscout::ObjectRef objRef = *(iter);
                osmscout::WayRef wayRef;

                searching->searchWay(objRef.GetId(), wayRef);

                if (wayRef.Valid()) {
                    /*
                    qDebug() << wayRef.Get()->EndIsJoint();
                    qDebug() << QString::fromStdString(wayRef.Get()->GetAttributes().GetName());
                    qDebug() << QString::fromStdString(wayRef.Get()->GetName());
                    qDebug() << wayRef.Get()->GetId();
                    qDebug() << wayRef.Get()->GetReferenceCount();
                    qDebug() << QString::fromStdString(wayRef.Get()->GetRefName());
                    qDebug() << wayRef.Get()->GetTagCount();
                    qDebug() << wayRef.Get()->GetType();
                    qDebug() << wayRef.Get()->GetWidth();
                    */

                    double lonMin;
                    double lonMax;
                    double latMin;
                    double latMax;

                    wayRef.Get()->GetBoundingBox(lonMin, lonMax, latMin, latMax);

                    std::vector<osmscout::NodeRef> nodes;
                    std::vector<osmscout::WayRef> ways;
                    std::vector<osmscout::WayRef> areas;
                    std::vector<osmscout::RelationRef> relationWays;
                    std::vector<osmscout::RelationRef> relationAreas;

                    //searching->searchObjects(lonMin, latMin, lonMax, latMax, nodes, ways, areas, relationWays, relationAreas);
/*
                    qDebug() << "Nodes: " << nodes.size();
                    qDebug() << "Ways: " << ways.size();
                    qDebug() << "Areas: " << areas.size();
                    qDebug() << "Relation ways: " << relationWays.size();
                    qDebug() << "Relation areas: " << relationAreas.size();
*/

                    for (int i = 0; i < areas.size(); i++) {
                        osmscout::WayRef areaRef;
                        osmscout::WayRef area = areas.at(i);
                        searching->searchWay(area.Get()->GetId(), areaRef);

                        if (areaRef.Valid()) {
                            // in poinst should be buildings!
                            std::vector<osmscout::Point> points = areaRef.Get()->nodes;

                            for (int j = 0; j < points.size(); j++) {
                                osmscout::Point point = points.at(j);
                                osmscout::NodeRef pointRef;
                                searching->searchNode(point.GetId(), pointRef);

                                if (pointRef.Valid()) {


                                    //if (pointRef.Get()->GetTagCount() > 0)
                                    //    qDebug() << pointRef.Get()->GetId() << " | " << QString::fromStdString(pointRef.Get()->GetTagValue(0));
                               }

                                osmscout::WayRef waj;
                                searching->searchWay(point.GetId(), waj);

                                if (waj.Valid()) {
                                    for (int j = 0; j < waj.Get()->GetTagCount(); j++)
                                        qDebug() << waj.Get()->GetTagKey(j) << " / " << QString(waj.Get()->GetTagValue(j).c_str());
                                    if (waj.Get()->GetTagCount() > 0)
                                        qDebug() << "\n";
                                }

                            }

                        }
                    }


                    for (int i = 0; i < ways.size(); i++) {
                        osmscout::WayRef way = ways.at(i);
                        osmscout::WayRef way2;

                        searching->searchWay(way.Get()->GetId(), way2);
                        //qDebug() << QString::fromStdString(way2.Get()->GetTagValue(0));

                        if (way2.Valid()) {
                            std::vector<osmscout::Point> points = way.Get()->nodes;

                            for (int j = 0; j < points.size(); j++) {
                                osmscout::Point point = points.at(j);
                                osmscout::NodeRef pointRef;
                                searching->searchNode(point.GetId(), pointRef);

                                osmscout::WayRef way3;
                                searching->searchWay(point.GetId(), way3);

                                if (way3.Valid()) {
                                //   qDebug() << QString::fromStdString(way3.Get()->GetTagValue(0));
                                }

                                if (pointRef.Valid()) {

                                    if (pointRef.Get()->GetTagCount() > 0)
                                        qDebug() << pointRef.Get()->GetId() << " | " << QString(pointRef.Get()->GetTagValue(0).c_str());
                               }

                            }
                        }


                    }


                }
            }
            break;

        deafult: std::cerr << "Error! Searching type is not initialized!"; break;
        }

        ui->treeWidget->clear();
    }
}

/*
  INFO click
  */
void PointSelectionWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    switch (searchingType) {
    case REGION_SEARCH:
        region = searchRegion(item->text(ID_COLUMN).toInt());
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

            infoWin->setName(region.name);

            searching->searchNode(region.reference.GetId(), cityRef);

            lat = cityRef.Get()->GetLat();
            lon = cityRef.Get()->GetLon();

            infoWin->setCoordinates(lat, lon);
            infoWin->setZoom(osmscout::magCity);
            infoWin->setVisible(true);
            //infoWin->setZoom(osmscout::magWorld);

            //osmscout::NodeRef node;
            //searching->database->GetNode(region.reference.GetId(), node);
            //infoWin->setCoordinates();

        break;

        case STREET_SEARCH:
            infoWin = new InfoWindow(NavigationWindow::main);

            osmscout::WayRef wayRef;

            int id = item->text(0).toInt();
            searching->searchWay(id, wayRef);


            infoWin->setName(QString::fromUtf8(wayRef.Get()->GetName().c_str()));

            wayRef.Get()->GetCenter(lat, lon);

            infoWin->setCoordinates(lat, lon);
            infoWin->setZoom(osmscout::magStreet);

            infoWin->setVisible(true);

            //infoWin->setMapRenderAreaSize(infoWin->getSize());
            //infoWin->setMapRender();
            //infoWin->forceMapWidgetRepaint();
            break;
        }

    }
}
/*
void PointSelectionWindow::fillPOIWidget(QString type)
{
    QVector<osmscout::NodeRef> poiRef;

    double cx = currentLocation.x();
    double cy = currentLocation.y();

    searching->searchPoi(cx, cy, 0, type, poiRef);

    QVector<double> distances;
    QVector<osmscout::NodeRef> poiRefSorted;

    for (int i = 0; i < poiRef.size(); i++) {
        osmscout::NodeRef node = poiRef.at(i);

        double dist = searching->CalculateDistance(cx, cy, node.Get()->GetLon(), node.Get()->GetLat());

        int j = 0;
        for (; j < distances.size(); j++) {
            if (dist < distances.at(j))
                break;
        }

        distances.insert(j, dist);
        poiRefSorted.insert(j, node);
    }

    QTreeWidgetItem *item;

    for (int i = 0; i < poiRefSorted.size(); i++) {
        item = new QTreeWidgetItem(ui->poiTreeWidget);

        osmscout::NodeRef node = poiRefSorted.at(i);

        QString name = "";

        if (node.Get()->GetTagCount() > 1) {
            name = QString::fromUtf8(node.Get()->GetTagValue(1).c_str());
        }

        //double distance = searching->calculateDistance(cx, cy, node.Get()->GetLon(), node.Get()->GetLat());

        item->setText(ID_COLUMN, QString::number(node.Get()->GetId()));
        item->setText(NAME_COLUMN, name);
        item->setText(PATH_COLUMN, QString::number(distances.at(i)) + " km");
        item->setText(INFO_COLUMN, "INFO");

        ui->poiTreeWidget->resizeColumnToContents(NAME_COLUMN);
        ui->poiTreeWidget->resizeColumnToContents(PATH_COLUMN);
    }
}
*/

void PointSelectionWindow::fillPOIWidget(int type, QString name)
{
    QPointF here = NavigationWindow::main->getCoordinates();
    double cx = here.x();
    double cy = here.y();

    PiLibocik::PoiFilePPOI poiDB;

    double x1 = cx;
    double x2 = cx;
    double y1 = cy;
    double y2 = cy;

    int limit = findPoisAreaLimit;
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
    }while(poiList.size()<findPoisCount && limit > 0);

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
        item->setText(NAME_COLUMN, poiList.at(it.value()).getName().toLatin1());
        item->setText(PATH_COLUMN, distance);
        item->setText(INFO_COLUMN, "INFO");
    }
    ui->poiTreeWidget->setColumnWidth(NAME_COLUMN,300);
  //  ui->poiTreeWidget->


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
    //int selectedId = ui->poiTreeWidget->currentItem()->text(ID_COLUMN).toInt();

    std::vector<osmscout::Routing::Step> route;

    double lons[12];
    double lats[12];
    double ids[12];
    bool cross[12];
    for (int i = 0; i < 12; i++)
        cross[i] = false;

    ids[0] = 163019177;
    ids[1] = 163019169;
    ids[2] = 700335113;
    ids[3] = 354688468;
    ids[4] = 163019162;
    ids[5] = 361263007;
    ids[6] = 163019154;
    ids[7] = 357534549;
    ids[8] = 354688480;
    ids[9] = 354688470;
    ids[10] = 173358982;
    ids[11] = 1354742039;

    lons[0] = 17.0200428;
    lons[1] = 17.0173305;
    lons[2] = 17.0173006;
    lons[3] = 17.0172331;
    lons[4] = 17.016984;
    lons[5] = 17.0168671;
    lons[6] = 17.0163112;
    lons[7] = 17.0162949;
    lons[8] = 17.0162113;
    lons[9] = 17.0160545;
    lons[10] = 17.0154863;
    lons[11] = 17.0152651;

    lats[0] = 51.0934354;
    lats[1] = 51.0911184;
    lats[2] = 51.0911032;
    lats[3] = 51.0910688;
    lats[4] = 51.0910281;
    lats[5] = 51.0910583;
    lats[6] = 51.0910981;
    lats[7] = 51.0911919;
    lats[8] = 51.0915246;
    lats[9] = 51.0921023;
    lats[10] = 51.0943413;
    lats[11] = 51.0951559;

    cross[6] = true;
    cross[8] = true;
    cross[9] = true;
    cross[10] = true;
    cross[11] = true;

    for (int i = 0; i < 12; i++) {
        osmscout::Routing::Step node;
        node.id = ids[i];
        node.lat = lats[i];
        node.lon = lons[i];
        node.crossing = cross[i];

        route.push_back(node);
    }

    NavigationWindow *par = dynamic_cast<NavigationWindow*>(parent());
    par->setRoute(QVector<osmscout::Routing::Step>::fromStdVector(route));

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
    }
}

void PointSelectionWindow::on_poiTreeWidget_itemSelectionChanged()
{
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
    StorePoint sp = historyPoints.at(ui->historyList->selectedItems().at(0)->text(0).toInt());
    retLon = sp.getLon();
    retLat = sp.getLat();
    retName = sp.getName();
}

void PointSelectionWindow::on_favouriteList_itemSelectionChanged()
{
    StorePoint sp = favouritePoints.at(ui->favouriteList->selectedItems().at(0)->text(0).toInt());
    retLon = sp.getLon();
    retLat = sp.getLat();
    retName = sp.getName();
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
    ui->historyList->setColumnCount(COLUMNS_COUNT);
    ui->historyList->hideColumn(ID_COLUMN);
    ui->historyList->header()->hide();
    foreach(StorePoint sp, historyPoints)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->historyList);
        item->setText(ID_COLUMN, QString::number(sp.getPos()));
        item->setText(NAME_COLUMN, sp.getName());
        item->setText(PATH_COLUMN, "0km");
        item->setText(INFO_COLUMN, "INFO");
    }
}

void PointSelectionWindow::initFavouriteList()
{
    favouritePoints = Settings::getInstance()->getFavouritePoints();
    ui->favouriteList->setColumnCount(COLUMNS_COUNT);
    ui->favouriteList->hideColumn(ID_COLUMN);
    ui->favouriteList->header()->hide();
    foreach(StorePoint sp, favouritePoints)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->favouriteList);
        item->setText(ID_COLUMN, QString::number(sp.getPos()));
        item->setText(NAME_COLUMN, sp.getName());
        item->setText(PATH_COLUMN, "0km");
        item->setText(INFO_COLUMN, "INFO");
    }
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
