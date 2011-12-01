#ifndef PLUGINSWINDOW_H
#define PLUGINSWINDOW_H

#include "qfullscreenframe.h"


class NavigationWindow;

namespace Ui {
    class PluginsWindow;
}

class PluginsWindow : public QFullScreenFrame
{
    Q_OBJECT

public:
    explicit PluginsWindow(NavigationWindow *parent = 0);
    ~PluginsWindow();

private slots:
    void on_okButton_clicked();
signals:
    void closed();

private:
    Ui::PluginsWindow *ui;
};

#endif // PLUGINSWINDOW_H
