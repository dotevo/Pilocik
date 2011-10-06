#ifndef POINTSELECTIONWINDOW_H
#define POINTSELECTIONWINDOW_H

#include <QFrame>

namespace Ui {
    class PointSelectionWindow;
}

class PointSelectionWindow : public QFrame
{
    Q_OBJECT

public:
    explicit PointSelectionWindow(QWidget *parent = 0);
    ~PointSelectionWindow();

private:
    Ui::PointSelectionWindow *ui;
};

#endif // POINTSELECTIONWINDOW_H
