#include "pointselectionwindow.h"
#include "ui_pointselectionwindow.h"
#include "navigationwindow.h"
#include "infowindow.h"

#include <QStandardItemModel>
#include <QAbstractItemView>
#include <QInputDialog>

#include <QDebug>
#include <osmscout/Searching.h>

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

        // [0] index column
        int id = item->text(0).toInt();

        int index = -1;

        switch (searchingType) {
        case REGION_SEARCH:
            for (int i = 0; i < regions.size(); i++) {
                if (id == regions.at(i).reference.GetId()) {
                    index = i;
                    break;
                }
            }
            region = regions.at(index);
            break;

        case STREET_SEARCH:
            std::list<osmscout::ObjectRef> refer = locations.at(0).references;

            QVector<osmscout::Point> points;

            for (std::list<osmscout::ObjectRef>::const_iterator iter = refer.begin();
                    iter != refer.end(); iter++) {

                osmscout::ObjectRef ref = *iter;
                osmscout::WayRef wayRef;
                searching->searchWay(ref.GetId(), wayRef);

                points += QVector<osmscout::Point>::fromStdVector(wayRef.Get()->nodes);
            }

            for (int i = 0; i < points.size(); i++)
            {
                //std::cout << points.at(i).GetId() << std::endl;
                osmscout::NodeRef node;

                searching->searchNode(points.at(i).GetId(), node);
                if (node.Valid())
                    std::cerr << "\t" << node.Get()->GetType() << std::endl;
            }

    //        std::cerr << "cuda na kiju" << std::endl;
    /*
            for (int i = 0; i < 999999999; i++) {
                osmscout::NodeRef node;
                searching->searchNode(i, node);
                if (node.Valid()) {
                    std::cout << i << ": is valid" << std::endl;
                } else {
                    std::cerr << i << ": is invalid" << std::endl;

                }
            }
    */
            break;

        deafult: std::cerr << "Error! Searching type is not initialized!"; break;
        }

        ui->treeWidget->clear();
    }
}

void PointSelectionWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    if (column == INFO_COLUMN) {
        InfoWindow *infoWin = new InfoWindow(this);

        infoWin->setName(region.name);

        double lat;
        double lon;

        switch (searchingType) {
        case REGION_SEARCH:
            infoWin = new InfoWindow(this);

            infoWin->setName(region.name);

            infoWin->setVisible(true);

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
