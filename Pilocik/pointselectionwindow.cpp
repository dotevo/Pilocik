#include "pointselectionwindow.h"
#include "ui_pointselectionwindow.h"
#include "navigationwindow.h"
#include "infowindow.h"

#include <QStandardItemModel>
#include <QAbstractItemView>
#include <QInputDialog>

#include <QDebug>
#include "osmscout/Searching.h"
#include "osmscout/Relation.h"

PointSelectionWindow::PointSelectionWindow(NavigationWindow *parent) :
    ui(new Ui::PointSelectionWindow),
    QFullScreenFrame(parent)
{
    ui->setupUi(this);
    sizeChanged(NavigationWindow::main);

    ui->treeWidget->setColumnCount(COLUMNS_COUNT);
    ui->treeWidget->hideColumn(ID_COLUMN);

    QStringList headers;
    headers << "Id" << "Name" << "Info";
    ui->treeWidget->setHeaderLabels(headers);
    ui->treeWidget->header()->hide();

    searching = new osmscout::Searching();
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
    emit ok_clicked();
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

                    searching->searchObjects(lonMin, latMin, lonMax, latMax, nodes, ways, areas, relationWays, relationAreas);
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

                            }

                        }
                    }


                    for (int i = 0; i < ways.size(); i++) {
                        osmscout::WayRef way = ways.at(i);
                        osmscout::WayRef way2;

                        for (int j = 0; j < way.Get()->GetTagCount(); j++)
                        //    qDebug() << way.Get()->GetTagKey(j) << " / " << QString::fromStdString(way.Get()->GetTagValue(j));
                        if (way.Get()->GetTagCount() > 0)
                            qDebug() << "\n";

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

                                if (way3.Valid())
                                //   qDebug() << QString::fromStdString(way3.Get()->GetTagValue(0));

                                if (pointRef.Valid()) {

                                   // if (pointRef.Get()->GetTagCount() > 0)
                                    //    qDebug() << pointRef.Get()->GetId() << " | " << QString::fromStdString(pointRef.Get()->GetTagValue(0));
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
        InfoWindow *infoWin = new InfoWindow(this);

        double lat;
        double lon;

        osmscout::NodeRef cityRef;

        switch (searchingType) {
        case REGION_SEARCH:
            infoWin = new InfoWindow(this);

            infoWin->setName(region.name);

            searching->searchNode(region.reference.GetId(), cityRef);

            lat = cityRef.Get()->GetLat();
            lon = cityRef.Get()->GetLon();

            infoWin->setCoordinates(lat, lon);
            infoWin->setVisible(true);
            //infoWin->setZoom(osmscout::magWorld);

            //osmscout::NodeRef node;
            //searching->database->GetNode(region.reference.GetId(), node);
            //infoWin->setCoordinates();

        break;

        case STREET_SEARCH:
            infoWin = new InfoWindow(this);

            osmscout::WayRef wayRef;

            int id = item->text(0).toInt();
            searching->searchWay(id, wayRef);


            infoWin->setName(QString::fromUtf8(wayRef.Get()->GetName().c_str()));

            wayRef.Get()->GetCenter(lat, lon);

            infoWin->setVisible(true);

            infoWin->setCoordinates(lat, lon);
            //infoWin->setMapRenderAreaSize(infoWin->getSize());
            infoWin->setMapRender();
            //infoWin->forceMapWidgetRepaint();
            break;
        }

    }
}