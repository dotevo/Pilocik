#ifndef INFOWINDOW_H
#define INFOWINDOW_H

#include <QFrame>
#include <QSize>

#include "qfullscreenframe.h"

namespace Ui {
    class InfoWindow;
}

/**
  @brief It is window with information about a object.
  */
class InfoWindow : public QFullScreenFrame
{
    Q_OBJECT

public:
    explicit InfoWindow(NavigationWindow *parent = 0);
    ~InfoWindow();

    /**
      @brief Settings coordinates for map render widget with preview of object.
      @param lat The latitude.
      @param lon The longitude.
      */
    void setCoordinates(const double lat, const double lon);

    /**
      @brief Settings zoom for map widget.
      @param zoom Zoom.
      */
    void setZoom(const int zoom);

    /**
      @brief Settings label text for object name.
      @param name Name of object.
      */
    void setName(const QString name);

    /**
      @brief Creating map render widget with preview of object.
      */
    void setMapRender();

    /**
      @brief Gets size of map render widget.
      @return Size of map render widget.
      */
    QSize getSize();

    /**
      @brief Forcing map render repaint.
      */
    void forceMapWidgetRepaint();

    /**
      @brief Sets poi description, gets as arg list of tags pairs.
      */
    void setDetails(QList < QPair <QString, QString> > tags);

    /**
      @brief Sets poi type and changes zoom to best for specified Poi.
      */
    void setPoiType(int type);

private slots:
    /**
      @brief Back button clicked. Closing info window.
      */
    void on_backButton_clicked();

private:
    Ui::InfoWindow *ui;

    QString name;
    double lon;
    double lat;

    void changeEvent(QEvent *);
};

#endif // INFOWINDOW_H
