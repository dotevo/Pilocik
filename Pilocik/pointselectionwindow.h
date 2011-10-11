#ifndef POINTSELECTIONWINDOW_H
#define POINTSELECTIONWINDOW_H

#include "qfullscreenframe.h"

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
signals:
    void ok_clicked();

private:
    Ui::PointSelectionWindow *ui;
};

#endif // POINTSELECTIONWINDOW_H
