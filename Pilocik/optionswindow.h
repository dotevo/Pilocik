#ifndef OPTIONSWINDOW_H
#define OPTIONSWINDOW_H

#include "qfullscreenframe.h"

namespace Ui {
    class OptionsWindow;
}

class MapOptionsWindow;
class GPSOptionsWindow;
class UserOptionsWindow;

class OptionsWindow : public QFullScreenFrame
{
    Q_OBJECT

public:
    explicit OptionsWindow(NavigationWindow *parent = 0);
    ~OptionsWindow();

private slots:
    void on_backButton_clicked();

    void on_mapButton_clicked();

    void on_GPSButton_clicked();

    void on_userButton_clicked();

    /**
     * @brief Shows options window after closing it's child window. (mapOptions, gpsOptions or userOptions)
     *
     */
    void optionWindowClosed();

signals:
    void windowClosed();

private:
    Ui::OptionsWindow *ui;
    MapOptionsWindow * moWin;
    GPSOptionsWindow *gpsWin;
    UserOptionsWindow *userWin;

    void changeEvent(QEvent *);
};

#endif // OPTIONSWINDOW_H
