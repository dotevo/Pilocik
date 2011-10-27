#ifndef MAPOPTIONSWINDOW_H
#define MAPOPTIONSWINDOW_H

#include "qfullscreenframe.h"

namespace Ui {
    class MapOptionsWindow;
}


class MapOptionsWindow : public QFullScreenFrame
{
    Q_OBJECT
public:
    explicit MapOptionsWindow(NavigationWindow *parent);
    ~MapOptionsWindow();

private slots:
    void on_okButton_clicked();

    void on_backButton_clicked();

private:
    Ui::MapOptionsWindow *ui;
};

#endif // MAPOPTIONSWINDOW_H
