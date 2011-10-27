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
    /**
      @brief OK button click. It choose actual hover object and close window.
      */
    void on_okButton_clicked();

    /**
      @brief Editing city line edit.
      @param arg1 New text.
      */
    void on_cityLineEdit_textChanged(const QString &arg1);

    /**
      @brief Editing street line edit.
      @param arg1 New text.
      */
    void on_streetLineEdit_textChanged(const QString &arg1);

    /**
      @brief Double clicked for item. Choosing location, works only when name column is clicked.
      @param item Choosing item.
      @param column Clicked column.
      */
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    /**
      @brief Clicked for item. It open info window for selected object.
      @param item Selected item.
      @param column Clicked column.
      */
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_poiOkButton_clicked();

signals:
    void ok_clicked();

private:
    static const int COUNTRY_SEARCH = 1;
    static const int REGION_SEARCH = 2;
    static const int STREET_SEARCH = 3;
    static const int NUMBER_SEARCH = 4;

    static const int ID_COLUMN = 0;
    static const int NAME_COLUMN = 1;
    static const int PATH_COLUMN = 2;
    static const int INFO_COLUMN = 3;
    static const int COLUMNS_COUNT = 4;

    Ui::PointSelectionWindow *ui;
    osmscout::Searching* searching;

    /**
      @brief Searching type. It defines what object type is searching now.
      */
    int searchingType;

    /**
      @brief Searching result regions.
      */
    QList<osmscout::AdminRegion> regions;

    /**
      @brief Searching result locations.
      */
    QList<osmscout::Location> locations;

    /**
      @brief Selected region.
      */
    osmscout::AdminRegion region;

    /**
      @brief Selected location.
      */
    osmscout::Location location;
};

#endif // POINTSELECTIONWINDOW_H
