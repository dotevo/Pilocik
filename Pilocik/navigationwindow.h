#ifndef NAVIGATIONWINDOW_H
#define NAVIGATIONWINDOW_H

#include <QMainWindow>

namespace Ui {
    class NavigationWindow;
}

class RouteWindow;
class OptionsWindow;

class NavigationWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit NavigationWindow(QWidget *parent = 0);
    ~NavigationWindow();
    void    resizeEvent ( QResizeEvent * event );

    static NavigationWindow *main;

private slots:
    void on_menuButton_clicked();
    void on_routeButton_clicked();
    void on_optionsButton_clicked();
    void on_zoomSlider_valueChanged(int value);

    void on_zoomSlider_sliderPressed();

    void on_zoomSlider_sliderReleased();

    void on_pushButton_4_clicked();
signals:
    void sizeChanged(QWidget *widget);
private:
    Ui::NavigationWindow *ui;
    void addWidgets();
    void addFrames();

    RouteWindow *routeWin;
    OptionsWindow *optionsWin;
};

#endif // NAVIGATIONWINDOW_H
