#ifndef POINTSELECTIONWINDOW_H
#define POINTSELECTIONWINDOW_H

#include "qfullscreenframe.h"

#include <QTreeWidgetItem>
#include <QMap>

#include <osmscout/Searching.h>

#include <pilibocik/poi.h>

#include <settings.h>



namespace Ui {
    class PointSelectionWindow;
}

class PointValue{

};

class PointSelectionWindow : public QFullScreenFrame
{
    Q_OBJECT

public:
    explicit PointSelectionWindow(NavigationWindow *parent = 0, double currentLocationX = 0, double currentLocationY = 0);
    ~PointSelectionWindow();

    /**
      @brief If PointSelectionWindow is used to select neares point, information is displayed at top of window.
      */
    void showNearestInfo();

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
      @brief Enables city line edit.
      */
    void on_cityEditBtn_clicked();

    /**
      @brief Clicked for item. It open info window for selected object.
      @param item Selected item.
      @param column Clicked column.
      */
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_nameLineEdit_textChanged(const QString &arg1);

    void on_poiOK_clicked();

    void on_addressOK_clicked();

    void on_historyOK_clicked();

    void on_favouriteOK_clicked();

    void on_findMore_clicked();

    void on_typeComboBox_currentIndexChanged(const QString &arg1);

    void on_poiTreeWidget_clicked(const QModelIndex &index);

    void on_poiTreeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_poiTreeWidget_itemSelectionChanged();

    void on_treeWidget_itemSelectionChanged();

    void on_historyList_itemSelectionChanged();

    void on_favouriteList_itemSelectionChanged();

    void on_historyList_itemClicked(QTreeWidgetItem *item, int column);

    void on_favouriteList_itemClicked(QTreeWidgetItem *item, int column);

    void on_tabWidget_currentChanged(int index);

    void on_showOpened_stateChanged(int state);

    void on_backBtn1_clicked();

    void on_backBtn2_clicked();

    void on_backBtn3_clicked();

    void on_backBtn4_clicked();

    void on_nearestButton_clicked();

    void on_nearestClrButton_clicked();

    void nearestSet(double lon, double lat, QString name);

    void pswClosed();

signals:
    void ok_clicked();
    void back_clicked();
    void positionChoosen(double lon, double lat, QString name);

private:
    static const int COUNTRY_SEARCH = 1;
    static const int REGION_SEARCH = 2;
    static const int STREET_SEARCH = 3;
    static const int NUMBER_SEARCH = 4;

    static const int ID_COLUMN = 0;
    static const int NAME_COLUMN = 1;
    static const int PATH_COLUMN = 2;
    static const int INFO_COLUMN = 3;
    static const int FAV_COLUMN = 4;
    static const int COLUMNS_COUNT = 5;

    double retLon, retLat;
    QString cityName, retName;

    PointSelectionWindow* npsw;

    int findPoisCount;
    int findPoisAreaLimit;
    QMap<int, QString> poiTypes;
    QList<PiLibocik::Poi> poiList;
    QList<StorePoint> historyPoints;
    QList<StorePoint> favouritePoints;
    QPair< QString, PiLibocik::Position > nearestPoint;

    /**
      @brief Coordinates of current location.
      */
    QPointF currentLocation;

    Ui::PointSelectionWindow *ui;
    osmscout::Searching* searching;

    /**
      @brief Searching region in <i>regions</i> with specified id.
      @param id Region id.
      */
    osmscout::AdminRegion searchRegion(const unsigned int id);

    /**
      @brieg Searching location in <i>locations</i> with specified name.
      @param name Location name.
      */
    osmscout::Location searchLocation(const QString locationName);

    /**
      @brief Sets POIs in widget with specified type.
      @param type Type of POIs.
      */
    void fillPOIWidget(int type, QString name);

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

    void changeEvent(QEvent *);

    void initHistoryList();

    void initFavouriteList();
};

#endif // POINTSELECTIONWINDOW_H
