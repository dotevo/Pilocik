#ifndef OPTIONSWINDOW_H
#define OPTIONSWINDOW_H

#include "qfullscreenframe.h"

namespace Ui {
    class OptionsWindow;
}

class MapOptionsWindow;
class GPSOptionsWindow;
class UserOptionsWindow;
class CreditsWindow;
class PluginsWindow;

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

    void on_creditsButton_clicked();

    void on_pluginsButton_clicked();

signals:
    void closed();

private:
    Ui::OptionsWindow *ui;
    MapOptionsWindow * moWin;
    GPSOptionsWindow *gpsWin;
    UserOptionsWindow *userWin;
    CreditsWindow *creditsWin;
    PluginsWindow *pluginsWin;

    void changeEvent(QEvent *);
};

#endif // OPTIONSWINDOW_H
