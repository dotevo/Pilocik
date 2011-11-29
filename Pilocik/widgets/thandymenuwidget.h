#ifndef THANDYMENUWIDGET_H
#define THANDYMENUWIDGET_H

#include <QWidget>
#include "tmovableframe.h"
#include "infowindow.h"

namespace Ui {
    class THandyMenuWidget;
}

class THandyMenuWidget : public TMovableFrame
{
    Q_OBJECT

public:
    explicit THandyMenuWidget(QWidget *parent = 0);
    ~THandyMenuWidget();
    /**
     * @brief Function that checks if there is need to show widget, and won't show widget if not.
     *
     * @param e - event
     */
    void showEvent(QShowEvent * e);
    /**
     * @brief Set poi button to show (or not).
     *
     */
    void setPOIClicked(bool clicked);
    /**
     * @brief Set poi window to show.
     *
     */
    void setPOIInfoWindow(InfoWindow * info);
    void setPosition(double lon, double lat);
    /**
     * @brief Sets widget to show.
     *
     */
    void showMenu(int x, int y);

private slots:
    void on_poInfoButton_clicked();
    void on_navFromButton_clicked();
    void on_navToButton_clicked();
    void on_cancelButton_clicked();

private:
    QPoint clicked;
    QPoint position;
    InfoWindow * infoWindow;
    bool poiClicked;
    bool shown;
    Ui::THandyMenuWidget *ui;
};

#endif // THANDYMENU_H
