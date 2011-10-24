#include "pointselectionwindow.h"
#include "ui_pointselectionwindow.h"
#include "navigationwindow.h"
#include "infowindow.h"

#include <QStandardItemModel>
#include <QAbstractItemView>
#include <QInputDialog>

#include <osmscout/Searching.h>

PointSelectionWindow::PointSelectionWindow(NavigationWindow *parent) :
    ui(new Ui::PointSelectionWindow),
    QFullScreenFrame(parent)
{
    ui->setupUi(this);
    sizeChanged(NavigationWindow::main);

    ui->treeWidget->setColumnCount(3);
    ui->treeWidget->hideColumn(0);
    //ui->treeWidget->resizeColumnToContents(1);
    ui->treeWidget->resizeColumnToContents(2);
    //ui->treeWidget->adjustSize();

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
    searchingType = REGION_SEARCH;

    ui->treeWidget->clear();

    if (text.length() > 2)
    {
        regions = searching->searchRegion(text);

        for (int i = 0; i < regions.size(); i++)
        {
            QTreeWidgetItem *item;

            if (i == 0) {
                item = new QTreeWidgetItem(ui->treeWidget);
                //ui->treeWidget->setCurrentItem();
            } else {
                item = new QTreeWidgetItem(ui->treeWidget, ui->treeWidget->currentItem());
            }

            item->setText(0, QString::number(regions.at(i).reference.GetId()));
            item->setText(1, regions.at(i).name);

            ui->treeWidget->setCurrentItem(item);
        }
    }
}

void PointSelectionWindow::on_streetLineEdit_textChanged(const QString &text)
{
    searchingType = STREET_SEARCH;

    ui->treeWidget->clear();

    locations = searching->searchLocation(text, region);

    for (int i = 0; i < locations.size(); i++)
    {
        QTreeWidgetItem *item;

        if (i == 0) {
            item = new QTreeWidgetItem(ui->treeWidget);
        } else {
            item = new QTreeWidgetItem(ui->treeWidget, ui->treeWidget->currentItem());
        }

        osmscout::ObjectRef loc = locations.at(i).reference.front();
        item->setText(0, QString::number(loc.GetId()));
        item->setText(1, locations.at(i).name.toUtf8());
        item->setText(2, "INFO");

    }
}

void PointSelectionWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if (column == 2) {
        InfoWindow *infoWin = new InfoWindow(this);

        osmscout::WayRef wayRef;

        int id = item->text(0).toInt();
        searching->searchWay(id, wayRef);

        infoWin->setName(QString::fromStdString(wayRef.Get()->GetName()));

        double lon;
        double lat;
        wayRef.Get()->GetCenter(lat, lon);

        infoWin->setVisible(true);

        infoWin->setCoordinates(lat, lon);
        infoWin->setMapRender();

    }
    else {
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
            std::list<osmscout::ObjectRef> refer = locations.at(0).reference;

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
                std::cout << points.at(i).GetId() << std::endl;
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
