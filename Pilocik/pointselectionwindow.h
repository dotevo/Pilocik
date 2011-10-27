#ifndef POINTSELECTIONWINDOW_H
#define POINTSELECTIONWINDOW_H

#include "qfullscreenframe.h"

#include <QTreeWidgetItem>

#include <osmscout/Searching.h>

namespace Ui {
    class PointSelectionWindow;
}

class PointValue{

};

class PointSelectionWindow : public QFullScreenFrame
{
    Q_OBJECT

public:
    explicit PointSelectionWindow(NavigationWindow *parent = 0);
    ~PointSelectionWindow();

private slots:
    void on_okButton_clicked();

    void on_cityLineEdit_textChanged(const QString &arg1);

    void on_streetLineEdit_textChanged(const QString &arg1);

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_poiOkButton_clicked();

signals:
    void ok_clicked();

private:
    static const int COUNTRY_SEARCH = 1;
    static const int REGION_SEARCH = 2;
    static const int STREET_SEARCH = 3;
    static const int NUMBER_SEARCH = 4;

    Ui::PointSelectionWindow *ui;
    osmscout::Searching* searching;

    int searchingType;

    QList<osmscout::AdminRegion> regions;
    QList<osmscout::Location> locations;

    osmscout::AdminRegion region;
    osmscout::Location location;
};

#endif // POINTSELECTIONWINDOW_H
