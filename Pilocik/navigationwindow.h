#ifndef NAVIGATIONWINDOW_H
#define NAVIGATIONWINDOW_H

#include <QMainWindow>

namespace Ui {
    class NavigationWindow;
}

class NavigationWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit NavigationWindow(QWidget *parent = 0);
    ~NavigationWindow();
    void    resizeEvent ( QResizeEvent * event );

private slots:
    void on_menuButton_clicked();

    void on_routeButton_clicked();

    void on_routeBackButton_clicked();

private:
    Ui::NavigationWindow *ui;
    void addWidgets();
};

#endif // NAVIGATIONWINDOW_H
