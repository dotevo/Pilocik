#ifndef MAPOPTIONSWINDOW_H
#define MAPOPTIONSWINDOW_H

#include "qfullscreenframe.h"
#include "pilibocik/poi.h"

namespace Ui {
    class MapOptionsWindow;
}


class MapOptionsWindow : public QFullScreenFrame
{
    Q_OBJECT
public:
    explicit MapOptionsWindow(NavigationWindow *parent);
    ~MapOptionsWindow();

    /**
      @brief Sets focus to other object than qlineedit to prevent keyboard widget autostart.
      */
    void setStartFocus();

private slots:
    void on_okButton_clicked();

    void on_backButton_clicked();

signals:
    void closed();

private:
    Ui::MapOptionsWindow *ui;
    QMap<int, PiLibocik::PoiDisplay> poiDisplaySettings;
    QMap<int,QString> poiTypes;
    void changeEvent(QEvent *);
};

#endif // MAPOPTIONSWINDOW_H
