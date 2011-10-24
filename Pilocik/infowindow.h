#ifndef INFOWINDOW_H
#define INFOWINDOW_H

#include <QFrame>

namespace Ui {
    class InfoWindow;
}

class InfoWindow : public QFrame
{
    Q_OBJECT

public:
    explicit InfoWindow(QWidget *parent = 0);
    ~InfoWindow();

    void setCoordinates(const double lat, const double lon);
    void setName(const QString name);

    void setMapRender();

private slots:
    void on_backButton_clicked();

private:
    Ui::InfoWindow *ui;

    QString name;
    double lon;
    double lat;
};

#endif // INFOWINDOW_H
