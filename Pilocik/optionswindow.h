#ifndef OPTIONSWINDOW_H
#define OPTIONSWINDOW_H

#include "qfullscreenframe.h"

namespace Ui {
    class OptionsWindow;
}

class MapOptionsWindow;

class OptionsWindow : public QFullScreenFrame
{
    Q_OBJECT

public:
    explicit OptionsWindow(NavigationWindow *parent = 0);
    ~OptionsWindow();

private slots:
    void on_backButton_clicked();

    void on_mapButton_clicked();

private:
    Ui::OptionsWindow *ui;
    MapOptionsWindow * moWin;
};

#endif // OPTIONSWINDOW_H
