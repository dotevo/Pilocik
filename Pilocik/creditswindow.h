#ifndef CREDITSWINDOW_H
#define CREDITSWINDOW_H

#include "qfullscreenframe.h"

namespace Ui {
    class CreditsWindow;
}

class CreditsWindow : public QFullScreenFrame
{
    Q_OBJECT

public:
    explicit CreditsWindow(NavigationWindow *parent = 0);
    ~CreditsWindow();

signals:
    void closed();

private slots:
    void on_backButton_clicked();

private:
    Ui::CreditsWindow *ui;
};

#endif // CREDITSWINDOW_H
