#ifndef GPSOPTIONSWINDOW_H
#define GPSOPTIONSWINDOW_H

#include <QFrame>

namespace Ui {
    class GPSOptionsWindow;
}

class GPSOptionsWindow : public QFrame
{
    Q_OBJECT

public:
    explicit GPSOptionsWindow(QWidget *parent = 0);
    ~GPSOptionsWindow();

private:
    Ui::GPSOptionsWindow *ui;
};

#endif // GPSOPTIONSWINDOW_H
